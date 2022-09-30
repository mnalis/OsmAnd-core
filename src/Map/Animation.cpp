#include "Animation.h"

#include "QtCommon.h"

#include "Utilities.h"

OsmAnd::GenericAnimation::GenericAnimation(
    const Key key_,
    const AnimatedValue animatedValue_,
    const float duration_,
    const float delay_,
    const TimingFunction timingFunction_,
    const std::shared_ptr<AnimationContext>& sharedContext_)
    : _isPaused(false)
    , _timePassed(0.0f)
    , _sharedContext(sharedContext_)
    , key(key_)
    , animatedValue(animatedValue_)
    , duration(duration_)
    , delay(delay_)
    , timingFunction(timingFunction_)
{
}

OsmAnd::GenericAnimation::~GenericAnimation()
{
}

float OsmAnd::GenericAnimation::properCast(const float value)
{
    return value;
}

double OsmAnd::GenericAnimation::properCast(const double value)
{
    return value;
}

double OsmAnd::GenericAnimation::properCast(const int32_t value)
{
    return static_cast<double>(value);
}

double OsmAnd::GenericAnimation::properCast(const int64_t value)
{
    return static_cast<double>(value);
}

OsmAnd::Animator::Key OsmAnd::GenericAnimation::getKey() const
{
    return key;
}

OsmAnd::Animator::AnimatedValue OsmAnd::GenericAnimation::getAnimatedValue() const
{
    return animatedValue;
}

float OsmAnd::GenericAnimation::getTimePassed() const
{
    QReadLocker scopedLocker(&_processLock);

    return _timePassed;
}

float OsmAnd::GenericAnimation::getDelay() const
{
    return delay;
}

float OsmAnd::GenericAnimation::getDuration() const
{
    return duration;
}

OsmAnd::Animator::TimingFunction OsmAnd::GenericAnimation::getTimingFunction() const
{
    return timingFunction;
}

void OsmAnd::GenericAnimation::pause()
{
    _isPaused = true;
}

void OsmAnd::GenericAnimation::resume()
{
    _isPaused = false;
}

bool OsmAnd::GenericAnimation::isPaused() const
{
    return _isPaused;
}

bool OsmAnd::GenericAnimation::isPlaying() const
{
    return (_timePassed >= delay) && ((_timePassed - delay) < duration);
}
