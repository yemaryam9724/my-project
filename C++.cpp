//Here's a Doctor Appointment Reservation System that handles registration, update, delete, and search functionality
#include <iostream>
using namespace std;

const int MAX_APPOINTMENTS = 50;

struct Appointment {
    int id;
    char patientName[50];
    char doctorName[50];
    int date;       // Format: DDMMYYYY
    int time;        // Format: HHMM
    bool isBooked;
};

Appointment appointments[MAX_APPOINTMENTS];
int appointmentCount = 0;

void addAppointment() {
    if(appointmentCount >= MAX_APPOINTMENTS) {
        cout << "Appointment slots full!\n";
        return;
    }

    Appointment newAppt;
    cout << "\n--- New Appointment ---\n";
    
    cout << "Enter appointment ID: ";
    cin >> newAppt.id;
    
    // Check for duplicate ID
    for(int i = 0; i < appointmentCount; i++) {
        if(appointments[i].id == newAppt.id) {
            cout << "ID already exists!\n";
            return;
        }
    }

    cout << "Patient name: ";
    cin.ignore();
    cin.getline(newAppt.patientName, 50);
    
    cout << "Doctor name: ";
    cin.getline(newAppt.doctorName, 50);
    
    cout << "Date (DDMMYYYY): ";
    cin >> newAppt.date;
    
    cout << "Time (HHMM): ";
    cin >> newAppt.time;
    
    newAppt.isBooked = true;
    
    appointments[appointmentCount++] = newAppt;
    cout << "Appointment booked successfully!\n";
}

void displayAllAppointments() {
    if(appointmentCount == 0) {
        cout << "No appointments found!\n";
        return;
    }

    cout << "\n--- All Appointments ---\n";
    for(int i = 0; i < appointmentCount; i++) {
        cout << "ID: " << appointments[i].id << endl;
        cout << "Patient: " << appointments[i].patientName << endl;
        cout << "Doctor: " << appointments[i].doctorName << endl;
        cout << "Date: " << appointments[i].date << endl;
        cout << "Time: " << appointments[i].time << endl;
        cout << "Status: " << (appointments[i].isBooked ? "Booked" : "Cancelled") << "\n\n";
    }
}

int findAppointment(int id) {
    for(int i = 0; i < appointmentCount; i++) {
        if(appointments[i].id == id) {
            return i;
        }
    }
    return -1;
}

void updateAppointment() {
    int id;
    cout << "Enter appointment ID to update: ";
    cin >> id;

    int index = findAppointment(id);
    if(index == -1) {
        cout << "Appointment not found!\n";
        return;
    }

    cout << "Enter new date (DDMMYYYY): ";
    cin >> appointments[index].date;
    
    cout << "Enter new time (HHMM): ";
    cin >> appointments[index].time;
    
    cout << "Appointment updated successfully!\n";
}

void cancelAppointment() {
    int id;
    cout << "Enter appointment ID to cancel: ";
    cin >> id;

    int index = findAppointment(id);
    if(index == -1) {
        cout << "Appointment not found!\n";
        return;
    }

    // Shift elements to remove the appointment
    for(int i = index; i < appointmentCount-1; i++) {
        appointments[i] = appointments[i+1];
    }
    appointmentCount--;
    
    cout << "Appointment cancelled successfully!\n";
}

void searchAppointment() {
    int id;
    cout << "Enter appointment ID to search: ";
    cin >> id;

    int index = findAppointment(id);
    if(index == -1) {
        cout << "Appointment not found!\n";
        return;
    }

    cout << "\n--- Appointment Details ---\n";
    cout << "ID: " << appointments[index].id << endl;
    cout << "Patient: " << appointments[index].patientName << endl;
    cout << "Doctor: " << appointments[index].doctorName << endl;
    cout << "Date: " << appointments[index].date << endl;
    cout << "Time: " << appointments[index].time << endl;
    cout << "Status: " << (appointments[index].isBooked ? "Booked" : "Cancelled") << "\n\n";
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    int choice;
    do {
        cout << "\nDoctor Appointment System\n";
        cout << "1. Book Appointment\n";
        cout << "2. View All Appointments\n";
        cout << "3. Update Appointment\n";
        cout << "4. Cancel Appointment\n";
        cout << "5. Search Appointment\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        clearInputBuffer();

        switch(choice) {
            case 1: addAppointment(); break;
            case 2: displayAllAppointments(); break;
            case 3: updateAppointment(); break;
            case 4: cancelAppointment(); break;
            case 5: searchAppointment(); break;
            case 6: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while(choice != 6);

    return 0;
}
