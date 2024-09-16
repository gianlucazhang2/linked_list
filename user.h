#include <string>
#include <fstream>


class User {
public:
	//constructor
	User(); 
	
	User(std::string n, int a, std::string gen, std::string ph, 
	std::string pro, std::string sch, float lat, float log, 
	bool prem, int min, int max, int dis, std::string p_gen, std::string like); 
	
//accessors
	std::string getName(); 
	int getAge();
	std::string getGender();
	std::string getPhone();
	std::string getProfession(); 
	std::string getSchool();
	double getLat(); 
	double getLong();
	bool getPremium();
	int getMin();
	int getMax();
	int getDistance();
	std::string getPref(); 
	std::string getLikes(); 
	
//operator overload
	friend std::ostream & operator << (std::ostream &out, const User &u);
	
private:
	std::string name, gender, phone, prof, school, pref, likes;
	int age, minAge, maxAge, distance;
	double latitude, longitude;
	bool premium;
};
	
//underscore to space string
	std::string space_(std::string text);

//sorting comparison
	bool sort_phone (User u1, User u2);