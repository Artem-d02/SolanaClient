#pragma once
#include <queue>

namespace NSolana {
    
    template <typename TTimeStamp>
    class TStats final {
    private:
        double sum_{ 0 };
        double sumSqr_{ 0 };
        TTimeStamp windowSize_;
        std::queue<std::pair<TTimeStamp, double>> vals_;

    public:
        TStats(const TTimeStamp& windowSize)
            : windowSize_(windowSize)
        {}

        double getNormalDeviationWithNewElement(const std::pair<TTimeStamp, double>& newElem) {
            if (vals_.empty() || (newElem.first - vals_.front().first) <= windowSize_) {
                sum_ += newElem.second;
                sumSqr_ += newElem.second * newElem.second;
                vals_.push(newElem);
            }
            else {
                while (!vals_.empty() && (newElem.first - vals_.front().first) > windowSize_) {
                    auto oldestValue = vals_.front().second;
                    vals_.pop();
                    sum_ -= oldestValue;
                    sumSqr_ -= oldestValue * oldestValue;
                }
            }

            return calculateStdDev();
        }

        TTimeStamp getWindowSize() const {
            return windowSize_;
        }
    
    private:
        double calculateStdDev() {
            double mean = sum_ / vals_.size();
            double variance = (sumSqr_ / vals_.size() - mean * mean);
            return std::sqrt(variance);
        }
    };

}   //  namespace NSolana