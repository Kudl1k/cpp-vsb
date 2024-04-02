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

    std::partial_sort(student_scores.begin(), student_scores.begin() + n, student_scores.end(), [](const auto& pair1, const auto& pair2) { 
        return pair1.second > pair2.second; 
    });

    return std::vector<std::pair<Student, Score>>(student_scores.begin(), student_scores.begin() + n);
}

size_t max_score_difference(const std::vector<Student> &students, const std::vector<Exam> &exams)
{
    std::vector<std::vector<Score>> all_scores(exams.size(), std::vector<Score>(students.size()));

    std::transform(exams.begin(),exams.end(),all_scores.begin(), [&students](const Exam &exam){
        std::vector<Score> scores(students.size());
        std::transform(students.begin(),students.end(),scores.begin(),[&exam](const Student &student){
            return calculate_score(student,exam);
        });
        std::sort(scores.begin(),scores.end(), std::greater<Score>());
        return scores;
    });

    std::vector<std::vector<Score>> all_differences(exams.size());

    std::transform(all_scores.begin(),all_scores.end(),all_differences.begin(),[](const std::vector<Score>& scores){
        std::vector<Score> differencies(scores.size() - 1);
        std::adjacent_difference(scores.begin(),scores.end(),differencies.begin());
        return differencies;
    });

    std::vector<Score> max_differencies(exams.size());
    std::transform(all_differences.begin(),all_differences.end(),max_differencies.end(),[](const std::vector<Score>& differencies){
        return *std::max_element(differencies.begin(),differencies.end());
    });


    return *std::max_element(max_differencies.begin(),max_differencies.end());
}

std::pair<std::unordered_set<Student>, std::unordered_set<Student>> filter_students(const std::vector<Student> &students, const std::vector<Exam> &exams)
{
    return std::pair<std::unordered_set<Student>, std::unordered_set<Student>>();
}

Leaderboard get_leaderboard_of_each_subject(const std::vector<Student> &students, const std::vector<Exam> &exams)
{
    return Leaderboard();
}
