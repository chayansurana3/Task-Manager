#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

class Task{
public:

    int id;
    std::string title;
    std::string description;
    std::string startDate;
    std::string dueDate;
    std::string status;
    int priority = -1;

    std::string tmToString(const std::tm& time) {
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << time.tm_mday << "/"
            << std::setw(2) << (time.tm_mon + 1) << "/20"
            << std::setw(2) << (time.tm_year % 100);
        return oss.str();
    }

    std::string getCurrentTime() {
        // Get the current system time
        std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm* localTime = std::localtime(&currentTime);
        return tmToString(*localTime);
    }

    void createTask(Task &T){
        std::cout << "Enter the task title: ";
        std::getline(std::cin, T.title);
        std::cout << "Enter the task description: ";
        std::getline(std::cin, T.description);
        std::cout << "Enter the task due date in DD/MM/YYYY format: ";
        std::getline(std::cin, T.dueDate);
        std::cout << "Enter the task priority (1 -> highest and 10 -> lowest): ";
        std::cin >> T.priority;
        std::getchar();
        std::cout << "Enter the task status: ";
        std::getline(std::cin, T.status);
        T.startDate = getCurrentTime();
    }

    std::string getTask() {return this->title;}
    std::string getDescription() {return this->description;}
    std::string getStartDate() {return this->startDate;}
    std::string getDueDate() {return this->dueDate;}
    std::string getStatus() {return this->status;}
    int getPriority() {return this->priority;}

    void DisplayTask(){
        std::cout << "\n-> Task Id: " << this->id;
        std::cout << ", Title: " << this->title; 
        std::cout << ", Description: " << this->description; 
        std::cout << ", Started On: " << this->startDate;
        std::cout << ", Due On: " << this->dueDate;
        std::cout << ", Priority: " << this->priority;
        std::cout << ", Status: " << this->status << std::endl; 
    }
};

#endif