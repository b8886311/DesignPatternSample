#ifndef AGVS_INCLUDE_UTIL_STOP_WATCH_H
#define AGVS_INCLUDE_UTIL_STOP_WATCH_H

#include <chrono>
namespace agvs {
class StopWatch
{
public:
    //! Constructor, default
    StopWatch()
        : startTime_(), endTime_(),
          diffTime_(0 ), totalTime_(0),
          running_(false), clockSessions_(0)
    {
    }

    // Destructor
    ~StopWatch() = default;

public:
    //! Start time measurement
    inline void start();

    //! Stop time measurement
    inline void stop();

    //! Reset time counters to zero
    inline void reset();

    //! Time in msec. after start. If the stop watch is still running (i.e. there
    //! was no call to stop()) then the elapsed time is returned, otherwise the
    //! time between the last start() and stop call is returned
    inline float getTime() const;

    //! Mean time to date based on the number of times the stopwatch has been
    //! _stopped_ (ie finished sessions) and the current total time
    inline float getAverageTime() const;

    inline bool isRunning() const;

private:
    // member variables

    //! Start of measurement
    //LARGE_INTEGER  startTime;
    std::chrono::steady_clock::time_point startTime_;

    //! End of measurement
    std::chrono::steady_clock::time_point endTime_;

    //! Time difference between the last start and stop
    float  diffTime_;

    //! TOTAL time difference between starts and stops
    float  totalTime_;

    //! flag if the stop watch is running
    bool running_;

    //! Number of times clock has been started
    //! and stopped to allow averaging
    int clockSessions_;
};

// functions, inlined

inline bool
StopWatch::isRunning() const{
    return running_;
}
////////////////////////////////////////////////////////////////////////////////
//! Start time measurement
////////////////////////////////////////////////////////////////////////////////
inline void
StopWatch::start() {
    startTime_ = std::chrono::steady_clock::now();
    running_ = true;
}

////////////////////////////////////////////////////////////////////////////////
//! Stop time measurement and increment add to the current diffTime_ summation
//! variable. Also increment the number of times this clock has been run.
////////////////////////////////////////////////////////////////////////////////
inline void
StopWatch::stop() {
    endTime_ = std::chrono::steady_clock::now();
    diffTime_ = (float)(std::chrono::duration_cast<std::chrono::microseconds>(endTime_ - startTime_).count()) / 1000.0f;

    totalTime_ += diffTime_;
    clockSessions_++;
    running_ = false;
}

////////////////////////////////////////////////////////////////////////////////
//! Reset the timer to 0. Does not change the timer running_ state but does
//! recapture this point in time as the current start time if it is running_.
////////////////////////////////////////////////////////////////////////////////
inline void
StopWatch::reset() {
    diffTime_ = 0;
    totalTime_ = 0;
    clockSessions_ = 0;

    if (running_) {
        startTime_ = std::chrono::steady_clock::now();
    }
}

////////////////////////////////////////////////////////////////////////////////
//! Time in msec. after start. If the stop watch is still running (i.e. there
//! was no call to stop()) then the elapsed time is returned added to the
//! current diffTime_ sum, otherwise the current summed time difference alone
//! is returned.
////////////////////////////////////////////////////////////////////////////////
inline float
StopWatch::getTime() const {
    // Return the TOTAL time to date
    float retval = totalTime_;

    if (running_) {
        retval += (float)(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - startTime_).count()) / 1000.0f;
    }

    return retval;
}

////////////////////////////////////////////////////////////////////////////////
//! Time in msec. for a single run based on the total number of COMPLETED runs
//! and the total time.
////////////////////////////////////////////////////////////////////////////////
inline float
StopWatch::getAverageTime() const {
    return (clockSessions_ > 0) ? (totalTime_ / static_cast<float>(clockSessions_)) : 0.0f;
}

}   //namespace agvs

#endif