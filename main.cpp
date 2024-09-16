#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <cmath>
#include <vector>
#include "user.h"
//linked list implementation
class LinkedList {
	struct Node {
		User user;
		Node* next;
		Node(const User& u) : user(u), next(nullptr) {}
	};
	Node* head;
public:
	LinkedList() : head(nullptr) {}

    void insertNode(const User& user) {
        Node* newNode = new Node(user);
        if (!head) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    void printList(std::ostream& output) {
        Node* current = head;
        while (current) {
            output << current->user;
            current = current->next;
        }
    }
	
	//find user
	User findUser(std::string number) {
		Node* tmp = head;
		while (tmp != nullptr) {
			if ((tmp->user).getPhone() == number) {
				return tmp->user;
			}
			tmp = tmp->next;
		}
		return User();
	}
	
	//find profile preferences
	bool isPref(User user1, User user2) {
		if (user1.getPref() == user2.getGender() || user1.getPref() == "Both") {
			if (user1.getMin() <= user2.getAge() && user1.getMax() >= user2.getAge()) {
				int distance = calculateDistance(user1.getLat(), user1.getLong(), user2.getLat(), user2.getLong()); 
				if (distance <= user1.getDistance()) {return true;}
				else {return false;}
			}
			else {return false;}
		}
		else {return false;}
	}
	
	//find users that fit users preferences
	void findPref(User user, std::ofstream& out) {
		Node* temp = head;
		bool noPref = true; 
		while (temp != NULL) {
			if (isPref(user, temp->user) && user.getPhone() != temp->user.getPhone()) {
				out << temp->user; 
				noPref = false; 
			}
			temp = temp->next;
		}
		if (noPref) {out << "There are no users matching with your preference at this moment.\n"<<std::endl;}
	}
	
	//find matches: both users have each other on their likes
	std::vector<User> findMatch(User user) {
		std::vector<std::string> likes = parseLikes(user.getLikes());
		std::vector<User> matches;

		for (std::string num : likes) { 
			User u = findUser(num);
			if (u.getLikes().find(user.getPhone()) != std::string::npos) {
				matches.push_back(u);
			}
		}
		return matches; 
	}
	
	void printMatches(std::ofstream& out, bool unmatch, User remove, std::vector<User> matches) {
		if (matches.empty()) {
			out << "You do not have any matches at this moment.\n"<<std::endl;
		} else {
			// Sort matches by phone number if 'sort_phone' is a valid comparison function
			std::sort(matches.begin(), matches.end(), sort_phone);
			if (unmatch && matches.size() == 1) {
				out << "You do not have any matches at this moment.\n"<<std::endl; 
			}
			else if (unmatch) {
				for (User u: matches) {
					if (u.getPhone() == remove.getPhone()) {continue;}
					else {out << u;}
				}
			}
			else {
				for (const User& u : matches) {
					out << u;
				}
			}	
		}
	}
	
	void findLikes(User user, std::ofstream& out) {
		if (!user.getPremium()) {out << "Only premium users can view who liked you.\n"<<std::endl;}
		else {
			Node* temp = head;
			bool hasLikes = false; 
			while (temp != NULL) {
				if (temp->user.getLikes().find(user.getPhone()) != std::string::npos) {
					hasLikes = true; 
					out << temp->user;
				}
				temp = temp->next;
			}
			if (!hasLikes) {out << "You have not received any likes so far.\n"<<std::endl;}
		}
	}
	
	~LinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
	
	// calculate the distance between two coordinates using Haversine formula
	double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
		const double radiusOfEarth = 6371.0; // Earth's radius in kilometers

		// convert latitude and longitude from degrees to radians
		lat1 *= M_PI / 180.0;
		lon1 *= M_PI / 180.0;
		lat2 *= M_PI / 180.0;
		lon2 *= M_PI / 180.0;

		// Haversine formula
		double dLat = lat2 - lat1;
		double dLon = lon2 - lon1;
		double a = sin(dLat / 2.0) * sin(dLat / 2.0) + cos(lat1) * cos(lat2) * sin(dLon / 2.0) * sin(dLon / 2.0);
		double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
		// distance in kilometers
		double distanceKM = radiusOfEarth * c;
		// convert it to distance in miles
		double distanceMiles = distanceKM * 0.621371;

		return distanceMiles;
	}
	
	//parses out the likes string into an array
	std::vector<std::string> parseLikes(std::string likes) {
		std::vector<std::string> numbers; 
		if (!likes.empty()) {
			std::stringstream ss(likes.substr(1));
			std::string number;
			while (std::getline(ss, number, '_')) {
				numbers.push_back(number);
			}
		}
		return numbers;
	}
	
};

//input file into linked list
LinkedList writeUsers(std::string input) {
	LinkedList users;
	std::ifstream istr(input); 
	std::string line; 
	while(std::getline(istr,line)) {
		std::istringstream iss(line);
        std::string name, gender, phone, prof, school, pref, likes;
        int age, minAge, maxAge, distance;
        float latitude, longitude;
        bool premium;

        iss >> name >> age >> gender >> phone >> prof >> school >> latitude >> longitude;
        iss >> std::boolalpha >> premium >> minAge >> maxAge >> distance >> pref;
        std::getline(iss, likes);

        User user(name, age, gender, phone, prof, school, latitude, longitude, premium, minAge, maxAge, distance, pref, likes);

        users.insertNode(user);
	}
	return users; 
}

int main(int argc, char *argv[]) {
	//correct number of parameters
	if (argc != 5 && argc != 6) {exit(0);}
	
	std::string input = argv[1]; 
	std::ofstream out(argv[2]);
	std::string number = argv[3]; 
	std::string action = argv[4]; 
	std::string otherNum; 
	
	//unmatch command case
	if (action == "unmatch" && argc == 6) {otherNum = argv[5];} 
	
	LinkedList users = writeUsers(input); 
	User user =  users.findUser(number);
	
	//show profiles
	if (action == "profile") {
		users.findPref(user, out); 
	}
	if (action == "match") {
		users.printMatches(out, false, user, users.findMatch(user)); 
	}
	
	if (action == "like") {
		users.findLikes(user, out); 
	}
	
	if (action == "unmatch") {
		User user2 = users.findUser(otherNum); 
		out << user.getName() << "'s match list:\n"<<std::endl; 
		users.printMatches(out, true, user2, users.findMatch(user));  
		
		out << "======\n" << std::endl; 
		out << user2.getName() << "'s match list:\n"<<std::endl; 
		users.printMatches(out, true, user, users.findMatch(user2)); 
	}

	
	return 0;
}