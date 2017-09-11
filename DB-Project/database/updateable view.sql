CREATE View updateable AS 
select DoctorId, FirstName, LastName, Speciality, ExperienceYears 
from doctor 
where ExperienceYears > 15 