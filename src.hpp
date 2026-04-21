#ifndef SRC_HPP_INCLUDED
#define SRC_HPP_INCLUDED
#include <cstring>
#include <string>
#include <vector>
#include <cstddef>

class BaseJudger {
public:
    BaseJudger(std::size_t time_limit, std::size_t memory_limit, const char *answer)
        : time_limit_(time_limit), memory_limit_(memory_limit), score_(0) {
        if (answer) {
            std::size_t n = std::strlen(answer);
            answer_ = new char[n + 1];
            std::memcpy(answer_, answer, n + 1);
        } else {
            answer_ = new char[1];
            answer_[0] = '\0';
        }
    }

    virtual void Submit(std::size_t time, std::size_t memory, const char *output) = 0;

    std::size_t GetScore() const { return score_; }

    virtual ~BaseJudger() {
        delete[] answer_;
    };

protected:
    char *answer_;
    const std::size_t time_limit_;
    const std::size_t memory_limit_;
    std::size_t score_;

    virtual bool CheckAnswer(const char *output) const {
        // the output must equal to the answer
        return std::strcmp(answer_, output) == 0;
    }
};

class ICPCJudger : public BaseJudger {
public:
    ICPCJudger(std::size_t time_limit, std::size_t memory_limit, const char *answer)
        : BaseJudger(time_limit, memory_limit, answer) {}

    void Submit(std::size_t time, std::size_t memory, const char *output) override {
        std::size_t cur = 0;
        if (time <= time_limit_ && memory <= memory_limit_ && CheckAnswer(output)) {
            cur = 100;
        } else {
            cur = 0;
        }
        if (cur > score_) score_ = cur;
    }
};

class OIJudger : public BaseJudger {
public:
    OIJudger(std::size_t time_limit, std::size_t memory_limit, const char *answer)
        : BaseJudger(time_limit, memory_limit, answer), submitted_(false) {}

    void Submit(std::size_t time, std::size_t memory, const char *output) override {
        if (submitted_) {
            score_ = 0;
            return;
        }
        submitted_ = true;
        if (time <= time_limit_ && memory <= memory_limit_ && CheckAnswerOI(output)) {
            score_ = 100;
        } else {
            score_ = 0;
        }
    }

protected:
    static void splitLinesTrimTrail(const char *cstr, std::vector<std::string> &out) {
        out.clear();
        if (cstr == nullptr) return;
        std::string s(cstr);
        std::size_t start = 0;
        while (start <= s.size()) {
            std::size_t pos = s.find('\n', start);
            std::string line;
            if (pos == std::string::npos) {
                line = s.substr(start);
                start = s.size() + 1;
            } else {
                line = s.substr(start, pos - start);
                start = pos + 1;
            }
            // trim trailing spaces only (ignore trailing spaces on each line)
            std::size_t endpos = line.find_last_not_of(' ');
            if (endpos == std::string::npos) {
                line.clear();
            } else {
                line.resize(endpos + 1);
            }
            out.push_back(line);
            if (start > s.size()) break;
        }
    }

    bool CheckAnswerOI(const char *output) const {
        std::vector<std::string> ans_lines;
        std::vector<std::string> out_lines;
        splitLinesTrimTrail(answer_, ans_lines);
        splitLinesTrimTrail(output, out_lines);
        if (ans_lines.size() != out_lines.size()) return false;
        for (std::size_t i = 0; i < ans_lines.size(); ++i) {
            if (ans_lines[i] != out_lines[i]) return false;
        }
        return true;
    }

private:
    bool submitted_;
};

class SpacialJudger : public BaseJudger {
public:
    SpacialJudger(std::size_t time_limit, std::size_t memory_limit, std::size_t full_score_time, std::size_t full_score_memory, const char *answer)
        : BaseJudger(time_limit, memory_limit, answer), fst_(full_score_time), fsm_(full_score_memory) {}

    void Submit(std::size_t time, std::size_t memory, const char *output) override {
        std::size_t cur = 0;
        if (time <= time_limit_ && memory <= memory_limit_ && CheckAnswer(output)) {
            int tscore = scoreLinear(time, fst_, time_limit_);
            int mscore = scoreLinear(memory, fsm_, memory_limit_);
            long long combined = 1LL * tscore * mscore;
            cur = static_cast<std::size_t>(combined / 100);
        } else {
            cur = 0;
        }
        if (cur > score_) score_ = cur;
    }

private:
    static int scoreLinear(std::size_t x, std::size_t full, std::size_t limit) {
        if (x >= limit) return 0;
        if (x <= full) return 100;
        std::size_t denom = (limit > full ? (limit - full) : 1);
        return static_cast<int>((100ull * (limit - x)) / denom);
    }

    std::size_t fst_;
    std::size_t fsm_;
};

#endif // SRC_HPP_INCLUDED
