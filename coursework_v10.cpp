#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <set>
#include <algorithm>

using namespace std;

// =============================== specification ==============================

class Person {
public:
    Person(string name, string surname);
    // Initialises the person name and surname.

    const string& get_name() const;
    // Return the name of the person.

    const string& get_surname() const;
    // Return the surname of the person.

    virtual bool has_telephone_p() const;
    // Test if a person has a telephone number.

    virtual bool has_email_p() const;
    // Test if a person has an e-mail address.

private:
    string _name;
    // Person's name.
    string _surname;
    // Person's surname.
};

class PersonWithEmail : virtual public Person {
public:
    PersonWithEmail(string name, string surname, string email);
    // Initialises the person name, surname and email address.

    bool has_email_p() const;
    // Test if a person has an email address.

    const string& get_email() const;
    // Return a person's telephone number.

    void set_email(string telephone);
    // Set a person's email address.

private:
    string _email;
    // Person's email address.
};

class PersonWithTelephone : virtual public Person {
public:
    PersonWithTelephone(string name, string surname, string telephone);
    // Initialises the person name, surname and telephone number.

    bool has_telephone_p() const;
    // Test if a person has a telephone number.

    const string& get_telephone() const;
    // Return a person's telephone number.

    void set_telephone(string telephone);
    // Set a person's telephone number.

private:
    string _telephone;
    // Person's telephone number.
};

class PersonWithTelephoneAndEmail : public PersonWithTelephone, public PersonWithEmail {
public:
    PersonWithTelephoneAndEmail(string name, string surname, string telephone, string email);
    // Initialises the person name, surname, telephone number and email address.
};

ostream& operator<<(ostream& os, const Person& p);
// Overload the insertion operator.

istream& read_person(istream& in, Person * & p);
// Read any type of person from an input stream and return it.

bool operator==(const Person& p1, const Person& p2);
// Overload the equal operator.

bool operator<(const Person& p1, const Person& p2);
// Overload the less than operator.

struct Comparator {
    bool operator()(const Person* p1, const Person* p2) const;
};
// A custom comparator for the set.

class Contacts {
public:
    void add(Person * p);
    // Adds a new contact / Updates an existing contact.

    void find(string name) const;
    // Searches for a contact by either a matching name or surname.

    void in_order() const;
    // Prints the contacts in alphabetic order (A–Z), according to 
    // surname and then name.

    void with_telephone(ostream& os) const;
    // Prints the contacts that have a telephone, in alphabetic order 
    // (A–Z), according to surname and then name.

private:
    set<Person*, Comparator> persons;
    // Set of pointers to the contacts.
    // Uses a custom comparator.
};


// ================================ testing ===================================

int main() {

    ifstream in_file("correct_input.txt");
    if (!in_file) {
        cout << "File not found." << endl;
    }

    Contacts c;
    Person *pp = nullptr;

    // testing "2.1 Inheritance" part 
    // 1.creating person objects from input.txt 
    while (read_person(in_file, pp) && pp) {
        // 2.then printing out
        cout << *pp << endl;

        // testing "2.2 Containers" part
        // adding contacts from file correct_input.txt
        c.add(pp);
    }

    // testing "2.3 Generic Algorithms" part
    cout << "=========================================================" << endl;
    c.find("John");

    cout << "=========================================================" << endl;
    c.in_order();

    cout << "=========================================================" << endl;
    c.with_telephone(cout);

    return 0;
}


// =============================== implementation =============================

Person::Person(string name, string surname) : _name(name), _surname(surname) {
}

const string& Person::get_name() const {
    return _name;
}

const string& Person::get_surname() const {
    return _surname;
}

bool Person::has_telephone_p() const {
    return false;
}

bool Person::has_email_p() const {
    return false;
}

ostream& operator<<(ostream& os, const Person & p) {
    os << "<person S " << p.get_surname() << " N " << p.get_name();
    if (p.has_telephone_p())
        os << " T " << (dynamic_cast<const PersonWithTelephone&> (p)).get_telephone();
    if (p.has_email_p())
        os << " E " << (dynamic_cast<const PersonWithEmail&> (p)).get_email();
    return os << '>';
}

istream& read_person(istream& in, Person * & p) {
    char c;
    if (in >> c && c == '<') {
        string sentence;
        getline(in, sentence, '>');
        istringstream iss(sentence);
        string word;
        vector<string> words;
        while (iss >> word) {
            words.push_back(word);
        }

        if (words[0] != "person" || words[1] != "S" || words[3] != "N") {
            in.setstate(ios::badbit);
        }

        switch (words.size()) {
            case 5:
                p = new Person(words[4], words[2]);
                break;
            case 7:
                if (words[6].find('@') == string::npos) {
                    p = new PersonWithTelephone(words[4], words[2], words[6]);
                    break;
                } else {
                    p = new PersonWithEmail(words[4], words[2], words[6]);
                    break;
                }
            case 9:
                p = new PersonWithTelephoneAndEmail(words[4], words[2], words[6], words[8]);
                break;
            default:
            {
                in.setstate(ios::badbit);
            }
        }
    } else {
        in.setstate(ios::badbit);
    }
    return in;
}

bool operator==(const Person& p1, const Person& p2) {
    if ((p1.get_surname() == p2.get_surname())
            && p1.get_name() == p2.get_name())
        return true;
    return false;
}

bool operator<(const Person& p1, const Person& p2) {
    if (p1.get_surname() < p2.get_surname())
        return true;
    if (p1.get_surname() == p2.get_surname()
            && p1.get_name() < p2.get_name())
        return true;
    return false;
}

PersonWithTelephone::PersonWithTelephone(string name, string surname, string telephone) :
Person(name, surname), _telephone(telephone) {
}

bool PersonWithTelephone::has_telephone_p() const {
    return true;
}

const string& PersonWithTelephone::get_telephone() const {
    return _telephone;
}

void PersonWithTelephone::set_telephone(string telephone) {
    _telephone = telephone;
}

PersonWithEmail::PersonWithEmail(string name, string surname, string email) :
Person(name, surname), _email(email) {
}

bool PersonWithEmail::has_email_p() const {
    return true;
}

const string& PersonWithEmail::get_email() const {
    return _email;
}

void PersonWithEmail::set_email(string email) {
    _email = email;
}

PersonWithTelephoneAndEmail::PersonWithTelephoneAndEmail(string name, string surname, string telephone, string email) :
Person(name, surname),
PersonWithTelephone(name, surname, telephone),
PersonWithEmail(name, surname, email) {
}

bool Comparator::operator()(const Person* p1, const Person* p2) const {
    return *p1 < *p2;
}

void Contacts::add(Person* p) {
    if (persons.find(p) != persons.end()) {
        persons.erase(p);
    }
    persons.insert(p);
}

void Contacts::find(string name) const {
    for_each(begin(persons), end(persons), [&](Person * p) {
        if (p->get_name() == name || p->get_surname() == name)
            cout << *p << endl;
    });
}

void Contacts::in_order() const {
    for_each(begin(persons), end(persons), [](Person * p) {
        cout << *p << endl;
    });
}

void Contacts::with_telephone(ostream& os) const {
    for_each(begin(persons), end(persons), [&](Person * p) {
        if (p->has_telephone_p())
            os << *p << endl;
    });
}