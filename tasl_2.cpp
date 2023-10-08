#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <cassert>
 
using namespace std;
 
class Domain {
public:
    Domain(const string& str)
        : domain_(str) {
        reverse(domain_.begin(), domain_.end());
        domain_.push_back('.');
    }
 
    bool operator==(const Domain& rhs) const {
        return (domain_ == rhs.domain_);
    }
    
    bool IsSubdomain(const Domain& other) const {
        if (other.domain_.size() <= domain_.size()) {
            return equal(other.domain_.begin(), other.domain_.end(), domain_.begin());
        }
        else {
            return false;
        }
    }
 
    string GetDomain() const {
        return domain_;
    }
 
private:
    string domain_;
};
 
class DomainChecker {
public:
    template <typename Iterator>
    DomainChecker(Iterator begin, Iterator end)
        : forbidden_domains_(begin, end) 
        {
        SortUniqueDomains();
        }
 
    bool IsForbidden(const Domain& domain) const {
        auto it = upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain, [](const Domain& lhs, const Domain& rhs){
                    return lhs.GetDomain() < rhs.GetDomain();
                });
        if (it == forbidden_domains_.begin()) {
            return false;
        } else {
            return domain.IsSubdomain(*prev(it));
        }
    }
    
private:
    vector<Domain> forbidden_domains_;
    
    void SortUniqueDomains() {
        sort(forbidden_domains_.begin(), forbidden_domains_.end(), [](const Domain& lhs, const Domain& rhs) {
            return lhs.GetDomain() < rhs.GetDomain(); 
            });
        auto last = unique(forbidden_domains_.begin(), forbidden_domains_.end(), [](const Domain& lhs, const Domain& rhs) {
                return rhs.IsSubdomain(lhs);
            });
        forbidden_domains_.erase(last, forbidden_domains_.end());
    }    
};
 
template <typename Number>
vector<Domain> ReadDomains(istream& in, Number num) {
    vector<Domain> domains;
    domains.reserve(num);
    string domain;
    for (size_t i = 0; i < num; ++i) {
        getline(in, domain);
        domains.emplace_back(domain);
    }
    return domains;
}
 
template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);
 
    Number num;
    std::istringstream(line) >> num;
 
    return num;
}

void TestDomain() {
    //Test Equality
    {
        Domain domain1("example.com");
        Domain domain2("com.example");
        Domain domain3("example.com");

        assert(domain1 == domain3);
        assert(!(domain1 == domain2));
    }
    //Test Subdomain
    {
        Domain domain1("com");
        Domain domain2("example.com");
        Domain domain3("example.org");

        assert(domain2.IsSubdomain(domain1));
        assert(!(domain1.IsSubdomain(domain3)));
    }
}

void TestDomainChecker() {
    vector<Domain> forb_dom = {Domain("sub.example.com"), Domain("test.example.com")};
    DomainChecker checker(forb_dom.begin(), forb_dom.end());
    //Test Forbidden
    {
        Domain test_domain1("sub.example.com");
        Domain test_domain2("test.example.com");
        Domain test_domain3("example.net");

        assert(checker.IsForbidden(test_domain1));
        assert(checker.IsForbidden(test_domain2));
        assert(!checker.IsForbidden(test_domain3));
    }    
}

void TestDomainFilter() {
    TestDomain();
    TestDomainChecker();    
}
 
int main() {
    TestDomainFilter();
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
 
    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}
