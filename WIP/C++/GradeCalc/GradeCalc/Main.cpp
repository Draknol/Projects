    #include <iostream>
    #include <fstream>

    using namespace std;

    int main()
    {
        bool read;
        float total = 0, mark, worth;
        fstream grade_save;
        string filename;
        cout << "Read (1) or Write (0): ";
        cin >> read;
        cout << "Enter subject: ";
        cin >> filename;
        filename += ".txt";
        if (!read)
        {
            grade_save.open(filename.c_str(), ios::in);
            int count;
            cout << "How many entries (negative to remove): ";
            cin >> count;

            if (count >= 0)
            {
                float mark, worth;
                for (int i = 0; i < count; i++)
                {
                    cout << "\nEnter mark: ";
                    cin >> mark;
                    cout << "Enter how much its worth: ";
                    cin >> worth;
                    grade_save << mark << " " << worth << " ";
                }
            }
            grade_save.close();
        }


        grade_save.open(filename.c_str(), ios::out | ios::app);

        while (grade_save >> mark >> worth)
        {
            cout << mark << " " << worth;
            total += mark * worth;
        }
        cout << total;

        grade_save.close();
        cin >> read;
        return 0;
    }