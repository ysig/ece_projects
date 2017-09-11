#1
delimiter //
CREATE TRIGGER `current_prescription_valid_doctor_trigger` BEFORE INSERT ON `prescription`
FOR EACH ROW
BEGIN
IF ((SELECT DoctorId FROM `patient` WHERE PatientId=NEW.PatientId) <> NEW.DoctorId) THEN
	SIGNAL SQLSTATE '45000' set message_text = 'Error: A patient can be perscripted only by his current doctor!';
END IF;
END
//
# Ελέγχει πριν από κάθε εισαγωγή στις συνταγές αν ένας ασθενής συνατγογραφείται από τον παρόν γιατρό.
# Αν όχι επιστρέφει αντίστοιχο μηνυμα λάθους και δεν αφήνει επιτρέπει στην εισαγωγή να γίνει.
# Αυτό το trigger προεκυψε από την περιγραφεί της βάσης στην άσκηση 1, όπου σε δύο σημεία αναγράφεται
# "Ένας ασθενής μπορεί να συνταγογραφείται μόνο από τον παρών γιατρό του"

#2
CREATE TRIGGER `doctors_patient_uniform_distribution_trigger` BEFORE INSERT ON `patient`
FOR EACH ROW
BEGIN
DECLARE max integer;
DECLARE min integer;
set @max := (SELECT COUNT(DoctorId) AS NumOfPatients FROM `patient` GROUP BY DoctorId ORDER BY COUNT(DoctorId) DESC LIMIT 1);
set @min := (SELECT COUNT(DoctorId) AS NumOfPatients FROM `patient` GROUP BY DoctorId ORDER BY COUNT(DoctorId) ASC LIMIT 1);
IF( (@max - @min >= 3) AND ((SELECT DoctorId AS NumOfPatients FROM `patient` GROUP BY DoctorId ORDER BY COUNT(DoctorId) DESC LIMIT 1) = NEW.DoctorId )) THEN
	set NEW.DoctorId = (SELECT DoctorId AS NumOfPatients FROM `patient` GROUP BY DoctorId ORDER BY COUNT(DoctorId) ASC LIMIT 1);
END IF;
END
//
# Διατηρεί μία ομοιόμορφη κατανομή μεταξύ ασθενών και γιατρών
# Αν γίνει μία εισαγωγή στη βάση ενός ασθενή σε ένα γιατρό ο οποίος έχει το μέγιστο για τη βάση πλήθος ασθενών
# και η διαφορά μεγίστου και ελαχίστου είναι μεγαλύτερη από 2 (επιλέχθηκε για να προσωμοιώνει μία δυνατότητα σχετικής προτίμησης)
# τότε μεταβάλουμε τον γιατρό ο οποίος ορίζεται για να συνατγογραφεί αυτόν τον ασθενή σε αυτόν με το ελάχιστο πλήθος ασθενών.
# Μετά από πολλές εισαγωγές (αν θεωρήσουμε την αρχικοποίηση της βάσης πριν το trigger) όλοι οι γιατροί θα έχουν το ίδιο πλήθος ασθενών
# με απόσταση το πολύ δύο (2) ασθενείς στο πλήθος.
