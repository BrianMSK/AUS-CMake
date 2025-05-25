#pragma once
class FilterAlgorithm {
public:
    template <typename containerT, typename Iterator, typename Predicate>
    containerT filter(Iterator itBeg_, Iterator itEnd_, Predicate predicate_) {
        containerT structureLoc;
        while (itBeg_ != itEnd_) {
            if (predicate_(*itBeg_)) {
                structureLoc.push_back(*itBeg_);
            }
            ++itBeg_;
        }
        return structureLoc;
    }
};
