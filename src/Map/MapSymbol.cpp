#include "MapSymbol.h"

OsmAnd::MapSymbol::MapSymbol(
    const std::shared_ptr<const MapSymbolsGroup>& group_,
    const bool isShareable_,
    const std::shared_ptr<const SkBitmap>& bitmap_,
    const int order_,
    const QString& content_,
    const LanguageId& languageId_,
    const PointI& minDistance_)
    : _bitmap(bitmap_)
    , group(group_)
    , groupPtr(group_.get())
    , isShareable(isShareable_)
    , bitmap(_bitmap)
    , order(order_)
    , content(content_)
    , languageId(languageId_)
    , minDistance(minDistance_)
{
}

OsmAnd::MapSymbol::~MapSymbol()
{
}

void OsmAnd::MapSymbol::releaseNonRetainedData()
{
    _bitmap.reset();
}
