	<?php	
		$nq = 10; //set number of queries
		// input must be between 1 and nq..
		$query = array(
			"SELECT P.Name, C.EndDate, C.Supervisor
           FROM pharmacy P JOIN contract C ON P.PharmacyId=C.PharmacyId 
           WHERE  C.EndDate LIKE '2017-03-%'"
			,
			"SELECT DISTINCT D.FirstName,D.LastName, C.Name
           FROM doctor D, prescription P, drug Dr, pharmaceuticalcompany C 
           WHERE  D.DoctorId=P.DoctorId AND P.DrugId=Dr.DrugId AND Dr.PharmaceuticalCompanyId=C.PharmaceuticalCompanyId"
			,
			"SELECT D.DrugId,MIN(S.Price) as MinPrice,P.Name
           FROM  pharmacy P, drug D, sell S 
           WHERE  P.PharmacyId=S.PharmacyId AND D.DrugId=S.DrugId
           GROUP BY D.DrugId
           ORDER BY MinPrice"
			,

           " SELECT P.PharmaceuticalCompanyId, COUNT(D.DrugId) AS AmountOfDrugs
            FROM PharmaceuticalCompany P, Drug D 
            WHERE  P.PharmaceuticalCompanyId=D.PharmaceuticalCompanyId
            GROUP BY P.PharmaceuticalCompanyId 
            ORDER BY AmountofDrugs"  
			,
			"SELECT P.PatientId, P.FirstName, P.LastName, Count(R.PatientId) as Count
			FROM patient P, prescription R
			WHERE (P.PatientId=R.PatientId)
			GROUP BY P.PatientId"
			,
			"SELECT DISTINCT d.Name, s.Price
			FROM sell s, drug d
			WHERE s.Price > (select avg(Price) from sell) and (s.DrugId=d.DrugId)
			GROUP BY d.Name"
			,
			"SELECT DISTINCT d.FirstName, d.LastName
			FROM doctor d 
			JOIN patient p
			ON p.DoctorId=d.DoctorId 
			WHERE d.ExperienceYears > 5 and p.Town='Athens'"
			,
			"SELECT doctor.FirstName, doctor.LastName, COUNT(*) AS \"NumberofPatients\" FROM (patient INNER JOIN doctor ON patient.DoctorId=doctor.DoctorId) GROUP BY doctor.FirstName, doctor.LastName having COUNT(*)>=2;"
			,
			"SELECT Name AS \"Drug Name\" FROM drug WHERE (NOT EXISTS (SELECT * FROM sell WHERE drug.DrugId=sell.DrugId))"
			,
			"(SELECT LastName AS \"Last Name\" FROM patient WHERE LastName LIKE '%nov%') UNION (SELECT LastName AS \"Last Name\" FROM doctor WHERE LastName LIKE '%nov%')"
			);
		$query_names = array("Join pharmacy and contract with end date in March","Pharmaceutical Companies favored by each doctor",
			"Every drug ordered by price","Pharmacy ordered by sells",
			"Number of drugs prescripted for patient",
			"Drugs with price higher than average",
			"Doctors with more than 5 years of experience and patients from Athens",
			"Doctors with two or more patient",			
			"Names of non available for selling drugs",
			"Names of doctors, patients having the russian \"nov\" syllab.");
	?>	