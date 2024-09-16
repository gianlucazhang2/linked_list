#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include "user.h"

//constructor
User::User() {
	name = ""; 
}

User::User(std::string n, int a, std::string gen, std::string ph, 
	std::string pro, std::string sch, float lat, float log, 
	bool prem, int min, int max, int dis, std::string p_gen, std::string like) {
		name = n; 
		age = a;
		gender = gen; 
		phone = ph; 
		prof = pro; 
		school = sch; 
		latitude = lat;
		longitude = log; 
		premium = prem; 
		minAge = min;
		maxAge = max;
		distance = dis;
		pref = p_gen;
		likes = like; 
	}
	
//accessors
std::string User::getName() {return name;}
int User::getAge() {return age;}
std::string User::getGender() {return gender;}
std::string User::getPhone() {return phone;}
std::string User::getProfession() {return prof;} 
std::string User::getSchool() {return school;}
double User::getLat() {return latitude;} 
double User::getLong() {return longitude;}
bool User::getPremium() {return premium;}
int User::getMin() {return minAge;}
int User::getMax() {return maxAge;}
int User::getDistance() {return distance;}
std::string User::getPref() {return pref;} 
std::string User::getLikes() {return likes;}

//operator overload
std::ostream & operator << (std::ostream &out, const User &u) {
	out << u.name << " " << u.age << std::endl;
	std::string prof = space_(u.prof);
	std::string school = space_(u.school); 
	if (prof != "Undisclosed") {out << prof << std::endl; }
	if (school != "Undisclosed") {out << school << std::endl;}
	out << "\n"; 
	return out; 
}

//replace underscore with space
std::string space_(std::string text) {
	std::replace(text.begin(), text.end(), '_', ' '); 
	return text;
}

//sorting function for users; increasing order
bool sort_phone (User u1, User u2) {
	if (u1.getPhone() < u2.getPhone()) {return true;}
	else {return false;}
}