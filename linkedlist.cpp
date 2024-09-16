#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include "linkedlist.h"

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
		while ((tmp->user).getPhone() != number) {
			tmp = tmp->next;
		}
		return tmp->user; 
	}
	
	//find profile preferences
	bool isPref(User user1, User user2) {
		if (user1.getPref() == user2.getGender() || user1.getPref() == "Both") {
			if (user1.getMin() <= user2.getAge() && user1.getMax() <= user2.getAge()) {
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
		while (temp != NULL) {
			if (isPref(user, temp->user)) {
				out << temp->user; 
			}
			temp = temp->next;
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
	
	~LinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};