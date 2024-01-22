#!/bin/bash

file_path="Businesses.csv"

while true; do
echo "[1] Επιλογή αρχείου επιχειρήσεων"
echo "[2] Προβολή στοιχείων επιχείρησης"
echo "[3] Αλλαγή στοιχείου επιχείρησης"
echo "[4] Προβολή αρχείου"
echo "[5] Αποθήκευση αρχείου"
echo "[6] Έξοδος"

read n

case $n in 
    1) 
    echo "Επιλέξτε αρχείο επιχειρήσεων"
    read -p "Εισάγετε το path του αρχείου. (enter για Businesses.csv): " file_path
    if [ -z "$file_path" ]; then
        file_path="Businesses.csv"
    fi
        
    if [ -e "$file_path" ]; then
        echo "Το επιλεγμένο αρχείο είναι: $file_path"
    else
        echo "Το αρχείο $file_path δεν υπάρχει\n"
    fi;;

    2) 
    echo "Επιλέξτε επιχείρηση"
    read input_id
    grep "$input_id" "$file_path";;                       
    3)
    echo "Επιλέξτε επιχείρηση"
    read input_id

    old_data=$(awk -F',' -v id="$input_id" '$1 == id {print $0}' "$file_path")

    if [ -z "$old_data" ]; then
        echo "Δεν βρέθηκε επιχείρηση με τον κωδικό $input_id"
    else
        echo "Παλιά στοιχεία επιχείρησης:"
        echo "$old_data"

        echo "Επιλέξτε το στοιχείο που θέλετε να αλλάξετε:"
        echo "1. ID"
        echo "2. BusinessName"
        echo "3. AddressLine2"
        echo "4. AddressLine3"
        echo "5. PostCode"
        echo "6. Longitude"
        echo "7. Latitude"
        read choice
        
        case $choice in
            1)
            echo "Εισάγετε το νέο ID"
            read new_id

            awk -F',' -v id="$input_id" -v new_id="$new_id" \
            '$1 == id {gsub($1, new_id)}1' "$file_path" > tmpfile && mv tmpfile "$file_path"

            echo "Νέα στοιχεία επιχείρησης:"
            grep "$new_id" "$file_path";;
            2)
            echo "Εισάγετε το νέο BusinessName"
            read new_businessname

            awk -F',' -v id="$input_id" -v new_businessname="$new_businessname" \
                '$1 == id {gsub($2, new_businessname)}1' "$file_path" > tmpfile && mv tmpfile "$file_path"

            echo "Νέα στοιχεία επιχείρησης:"
            grep "$input_id" "$file_path";;
            3)
            echo "Εισάγετε το νέο AddressLine2"
            read new_addressline2

            awk -F',' -v id="$input_id" -v new_addressline2="$new_addressline2" \
                '$1 == id {gsub($3, new_addressline2)}1' "$file_path" > tmpfile && mv tmpfile "$file_path"

            echo "Νέα στοιχεία επιχείρησης:"
            grep "$input_id" "$file_path";;
            4)
            echo "Εισάγετε το νέο AddressLine3"
            read new_addressline3

            awk -F',' -v id="$input_id" -v new_addressline3="$new_addressline3" \
                '$1 == id {gsub($4, new_addressline3)}1' "$file_path" > tmpfile && mv tmpfile "$file_path"

            echo "Νέα στοιχεία επιχείρησης:"
            grep "$input_id" "$file_path";;
            5)
            echo "Εισάγετε το νέο PostCode"
            read new_postcode

            awk -F',' -v id="$input_id" -v new_postcode="$new_postcode" \
                '$1 == id {gsub($5, new_postcode)}1' "$file_path" > tmpfile && mv tmpfile "$file_path"

            echo "Νέα στοιχεία επιχείρησης:"
            grep "$input_id" "$file_path";;
            6)
            echo "Εισάγετε το νέο Longitude"
            read new_longitude

            awk -F',' -v id="$input_id" -v new_longitude="$new_longitude" \
                '$1 == id {gsub($6, new_longitude)}1' "$file_path" > tmpfile && mv tmpfile "$file_path"

            echo "Νέα στοιχεία επιχείρησης:"
            grep "$input_id" "$file_path";;
            7)
            echo "Εισάγετε το νέο Latitude"
            read new_latitude

            awk -F',' -v id="$input_id" -v new_latitude="$new_latitude" \
                '$1 == id {gsub($7, new_latitude)}1' "$file_path" > tmpfile && mv tmpfile "$file_path"

            echo "Νέα στοιχεία επιχείρησης:"
            grep "$input_id" "$file_path";;
        esac
    fi;;
    4)
    more "$file_path";;
    5)
    read -p "Enter the path to save the file or press enter for default: " save_path
    if [[ -z "$save_path" ]]; then
        save_path="clients.csv"
    fi
    cp "$file_path" "$save_path"
    echo "File saved to $save_path";;
    6) 
    exit;;
esac

done
