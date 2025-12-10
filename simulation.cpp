#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Enter array size: ";
    cin >> n;

    int arr[n];

    // Initialize array
    cout << "Enter " << n << " elements:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Update element\n";
        cout << "2. Get value\n";
        cout << "3. Print array\n";
        cout << "4. Exit\n";
        cout << "Choose: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            int idx, val;
            cout << "Enter index and new value: ";
            cin >> idx >> val;

            if (idx >= 0 && idx < n) {
                arr[idx] = val;
                cout << "Updated!\n";
            } else {
                cout << "Invalid index!\n";
            }
        }

        else if (choice == 2) {
            int idx;
            cout << "Enter index: ";
            cin >> idx;

            if (idx >= 0 && idx < n) {
                cout << "Value at index " << idx << " = " << arr[idx] << "\n";
            } else {
                cout << "Invalid index!\n";
            }
        }

        else if (choice == 3) {
            cout << "Array: ";
            for (int i = 0; i < n; i++)
                cout << arr[i] << " ";
            cout << "\n";
        }

        else if (choice == 4) {
            break;
        }

        else {
            cout << "Invalid choice!\n";
        }
    }

    return 0;
}
