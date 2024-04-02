#include "tasks.h"

std::ostream& operator<<(std::ostream& os, const Student& student)
{
    os << student.name;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Exam& exam)
{
    os << exam.subject;
    return os;
}

Score calculate_score(Student student, Exam exam) {
    int math_diff = static_cast<int>(student.math_skill) * std::max(1, 100 - static_cast<int>(exam.math_difficulty));
    int coding_diff = static_cast<int>(student.coding_skill) * std::max(1, 100 - static_cast<int>(exam.coding_difficulty));

    int diff = math_diff + coding_diff;
    return diff / 50;
}

std::vector<std::pair<Student, Score>> find_best_n_students(std::vector<Student> students, const Exam &exam, size_t n)
{
    std::vector<Score> scores(students.size());

    std::transform(students.begin(), students.end(), scores.begin(), [&exam](const Student& student) {
        return calculate_score(student, exam); 
    });

    std::vector<std::pair<Student, Score>> student_scores(students.size());

    std::transform(students.begin(), students.end(), scores.begin(), student_scores.begin(),[](const Student& student, const Score& score) { 
        return std::make_pair(student, score);
    });

    std::sort(student_scores.begin(), student_scores.end(), [](const auto& pair1, const auto& pair2) { 
        return pair1.second > pair2.second; 
    });

    if (n > student_scores.size()) {
        n = student_scores.size();
    }

    return std::vector<std::pair<Student, Score>>(student_scores.begin(), student_scores.begin() + n);
}

size_t max_score_difference(const std::vector<Student>& students, const std::vector<Exam>& exams) {
    std::vector<size_t> max_diffs;

    std::transform(exams.begin(), exams.end(), std::back_inserter(max_diffs), [&](const Exam& exam) {
        std::vector<Score> scores;

        std::transform(students.begin(), students.end(), std::back_inserter(scores), [&](const Student& student) {
            return calculate_score(student, exam);
        });

        if (!scores.empty()) {
            auto minmax = std::minmax_element(scores.begin(), scores.end());
            return *minmax.second - *minmax.first;
        } else {
            return 0;
        }
    });

    return max_diffs.empty() ? 0 : *std::max_element(max_diffs.begin(), max_diffs.end());
}

std::pair<std::unordered_set<Student>, std::unordered_set<Student>> filter_students(const std::vector<Student> &students, const std::vector<Exam> &exams)
{
    std::unordered_set<Student> passed_all_exams;
    std::unordered_set<Student> passed_single_exam;

    std::for_each(students.begin(), students.end(), [&](const Student& student) {
        bool passed_all = std::all_of(exams.begin(), exams.end(), [&](const Exam& exam) {
            Score score = calculate_score(student, exam);
            return score >= 100;
        });

        bool passed_single = std::any_of(exams.begin(), exams.end(), [&](const Exam& exam) {
            Score score = calculate_score(student, exam);
            return score >= 100;
        });

        if (passed_all) {
            passed_all_exams.insert(student);
        }
        if (passed_single) {
            passed_single_exam.insert(student);
        }
    });

    return std::make_pair(passed_all_exams, passed_single_exam);
}

Leaderboard get_leaderboard_of_each_subject(const std::vector<Student>& students, const std::vector<Exam>& exams) {
    Leaderboard leaderboard;

    std::for_each(exams.begin(), exams.end(), [&](const Exam& exam) {
        std::vector<std::pair<Student, Score>> student_scores = find_best_n_students(students, exam, students.size());
        leaderboard.emplace(exam.subject, std::move(student_scores));
    });

    std::for_each(leaderboard.begin(), leaderboard.end(), [](auto& entry) {
        std::sort(entry.second.begin(), entry.second.end(), [](const auto& pair1, const auto& pair2) {
            if (pair1.second == pair2.second) {
                return pair1.first.name < pair2.first.name;
            }
            return pair1.second > pair2.second;
        });
    });

    return leaderboard;
}

