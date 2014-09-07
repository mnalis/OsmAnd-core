#include "OsmAndCore.h"

#include "stdlib_common.h"
#include <memory>

#include "QtExtensions.h"
#include <QCoreApplication>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QLocale>

#include "ignore_warnings_on_external_includes.h"
#include <gdal.h>
#include "restore_internal_warnings.h"

#include "ignore_warnings_on_external_includes.h"
#include <SkGraphics.h>
#include "restore_internal_warnings.h"

#include "Common.h"
#include "Logging.h"
#include "DefaultLogSink.h"
#include "OsmAndCore_private.h"
#include "ExplicitReferences.h"
#include "QMainThreadTaskHost.h"
#include "QMainThreadTaskEvent.h"
#include "ICU_private.h"
#include "TextRasterizer_private.h"
#include "MapSymbolIntersectionClassesRegistry_private.h"

#if defined(OSMAND_TARGET_OS_)
#   error CMAKE_TARGET_OS defined incorrectly
#endif // defined(OSMAND_TARGET_OS_)

#if defined(OSMAND_TARGET_CPU_ARCH_)
#   error CMAKE_TARGET_CPU_ARCH defined incorrectly
#endif // defined(OSMAND_TARGET_CPU_ARCH_)

#if defined(OSMAND_TARGET_CPU_ARCH_FAMILY_)
#   error CMAKE_TARGET_CPU_ARCH_FAMILY defined incorrectly
#endif // defined(OSMAND_TARGET_CPU_ARCH_FAMILY_)

#if defined(OSMAND_COMPILER_FAMILY_)
#   error CMAKE_COMPILER_FAMILY defined incorrectly
#endif // defined(OSMAND_COMPILER_FAMILY_)

namespace OsmAnd
{
    void initializeInAppThread();
    void releaseInAppThread();
    QThread* gMainThread;
    std::shared_ptr<QObject> gMainThreadRootObject;

    std::shared_ptr<const ICoreResourcesProvider> gCoreResourcesProvider;
}

int _dummyArgc = 1;
const char* _dummyArgs[] = { "osmand.core" };
std::shared_ptr<QCoreApplication> _qCoreApplication;
QMutex _qCoreApplicationThreadMutex;
QWaitCondition _qCoreApplicationThreadWaitCondition;
struct QCoreApplicationThread : public QThread
{
    QCoreApplicationThread()
        : wasInitialized(false)
    {
    }

    volatile bool wasInitialized;

    void run()
    {
        _qCoreApplication.reset(new QCoreApplication(_dummyArgc, const_cast<char**>(&_dummyArgs[0])));
        OsmAnd::initializeInAppThread();
        {
            QMutexLocker scopedLocker(&_qCoreApplicationThreadMutex);

            wasInitialized = true;

            _qCoreApplicationThreadWaitCondition.wakeAll();
        }
        QCoreApplication::exec();
        OsmAnd::releaseInAppThread();
        _qCoreApplication.reset();
    }
};
std::shared_ptr<QCoreApplicationThread> _qCoreApplicationThread;

OSMAND_CORE_API void OSMAND_CORE_CALL OsmAnd::InitializeCore(const std::shared_ptr<const ICoreResourcesProvider>& coreResourcesProvider)
{
    assert(coreResourcesProvider);
    gCoreResourcesProvider = coreResourcesProvider;

    Logger::get()->addLogSink(std::shared_ptr<ILogSink>(new DefaultLogSink()));

    InflateExplicitReferences();

    if (!QCoreApplication::instance())
    {
        LogPrintf(LogSeverityLevel::Info,
            "OsmAnd Core is initialized standalone, so going to create 'application' thread");
        _qCoreApplicationThread.reset(new QCoreApplicationThread());
        gMainThread = _qCoreApplicationThread.get();
        _qCoreApplicationThread->start();

        // Wait until global initialization will pass in that thread
        {
            QMutexLocker scopeLock(&_qCoreApplicationThreadMutex);
            while (!_qCoreApplicationThread->wasInitialized)
                REPEAT_UNTIL(_qCoreApplicationThreadWaitCondition.wait(&_qCoreApplicationThreadMutex));
        }
    }
    else
    {
        LogPrintf(LogSeverityLevel::Info,
            "OsmAnd Core is initialized inside a Qt application, so assuming that got called from application thread");
        gMainThread = QThread::currentThread();
        initializeInAppThread();
    }

    // GDAL
    GDALAllRegister();

    // SKIA
    SkGraphics::PurgeFontCache(); // This will initialize global glyph cache, since it fails to initialize concurrently

    // ICU
    ICU::initialize();

    // Qt
    (void)QLocale::system(); // This will initialize system locale, since it fails to initialize concurrently

    // Text rasterizer
    TextRasterizer_initializeGlobalInstance();

    // MapSymbol intersection classes registry
    MapSymbolIntersectionClassesRegistry_initializeGlobalInstance();
}

OSMAND_CORE_API const std::shared_ptr<const OsmAnd::ICoreResourcesProvider>& OSMAND_CORE_CALL OsmAnd::getCoreResourcesProvider()
{
    return gCoreResourcesProvider;
}

OSMAND_CORE_API void OSMAND_CORE_CALL OsmAnd::ReleaseCore()
{
    if (_qCoreApplicationThread)
    {
        QCoreApplication::exit();
        REPEAT_UNTIL(_qCoreApplicationThread->wait());
        _qCoreApplicationThread.reset();
    }
    else
    {
        releaseInAppThread();
    }

    // MapSymbol intersection classes registry
    MapSymbolIntersectionClassesRegistry_releaseGlobalInstance();

    // Text rasterizer
    TextRasterizer_releaseGlobalInstance();

    // ICU
    ICU::release();

    Logger::get()->flush();
    Logger::get()->removeAllLogSinks();
}

void OsmAnd::initializeInAppThread()
{
    gMainThreadRootObject.reset(new QMainThreadTaskHost());
}

void OsmAnd::releaseInAppThread()
{
    gMainThreadRootObject.reset();
}

#if defined(OSMAND_TARGET_OS_android)
//HACK: https://code.google.com/p/android/issues/detail?id=43819
//TODO: Remove this mess when it's going to be fixed
#include <iostream>
::std::ios_base::Init::Init()
{
}
#endif
