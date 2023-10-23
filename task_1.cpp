#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

class ReadingManager {
public:
    ReadingManager() :
        user_page_counts_(MAX_USER_COUNT_ + 1, 0),
        page_count_to_num_users_(MAX_PAGE_COUNT_ + 1, 0),
        num_users_(0) {}

    void Read(int user_id, int page_count) {
        int prev_page_count = user_page_counts_[user_id];
        if (!prev_page_count) {
            num_users_++;
        } else {
            page_count_to_num_users_[prev_page_count]--;
        }
        user_page_counts_[user_id] = page_count;
        page_count_to_num_users_[page_count]++;
    }

    double Cheer(int user_id) const {
        int page_count = user_page_counts_[user_id];
        if (!page_count)
            return 0;
        if (num_users_ == 1)
            return 1;
        int num_users_with_less_pages = 0;
        for (int i = 1; i < page_count; i++) {
            num_users_with_less_pages += page_count_to_num_users_[i];
        }
        return (num_users_with_less_pages) * 1.0 / (num_users_ - 1);
    }

private:
    static const int MAX_USER_COUNT_ = 100'000;
    static const int MAX_PAGE_COUNT_ = 1000;

    vector<int> user_page_counts_;
    vector<int> page_count_to_num_users_;
    unsigned int num_users_;
};

int main() {
    
    ReadingManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int user_id;
        cin >> user_id;

        if (query_type == "READ") {
            int page_count;
            cin >> page_count;
            manager.Read(user_id, page_count);
        } else if (query_type == "CHEER") {
            cout << setprecision(6) << manager.Cheer(user_id) << "\n";
        }
    }

    return 0;
}
