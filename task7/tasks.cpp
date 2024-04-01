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
    return std::vector<std::pair<Student, Score>>();
}

size_t max_score_difference(const std::vector<Student> &students, const std::vector<Exam> &exams)
{
    return size_t();
}

std::pair<std::unordered_set<Student>, std::unordered_set<Student>> filter_students(const std::vector<Student> &students, const std::vector<Exam> &exams)
{
    return std::pair<std::unordered_set<Student>, std::unordered_set<Student>>();
}

Leaderboard get_leaderboard_of_each_subject(const std::vector<Student> &students, const std::vector<Exam> &exams)
{
    return Leaderboard();
}
