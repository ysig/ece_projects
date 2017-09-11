-- phpMyAdmin SQL Dump
-- version 4.5.1
-- http://www.phpmyadmin.net
--
-- Host: 127.0.0.1
-- Generation Time: Mar 08, 2017 at 01:36 AM
-- Server version: 10.1.19-MariaDB
-- PHP Version: 5.6.28

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `ptest2`
--

-- --------------------------------------------------------

--
-- Stand-in structure for view `avgprice`
--
CREATE TABLE `avgprice` (
`DrugId` int(11)
,`DrugName` varchar(255)
,`AvgPrice` double(19,2)
);

-- --------------------------------------------------------

--
-- Table structure for table `contract`
--

CREATE TABLE `contract` (
  `PharmacyId` int(11) NOT NULL,
  `PharmaceuticalCompanyId` int(11) NOT NULL,
  `StartDate` date NOT NULL,
  `EndDate` date NOT NULL,
  `Text` text NOT NULL,
  `Supervisor` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `doctor`
--

CREATE TABLE `doctor` (
  `DoctorId` int(11) NOT NULL,
  `FirstName` varchar(255) NOT NULL,
  `LastName` varchar(255) NOT NULL,
  `Speciality` varchar(255) NOT NULL,
  `ExperienceYears` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `drug`
--

CREATE TABLE `drug` (
  `DrugId` int(11) NOT NULL,
  `Name` varchar(255) NOT NULL,
  `Formula` varchar(255) NOT NULL,
  `PharmaceuticalCompanyId` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `patient`
--

CREATE TABLE `patient` (
  `PatientId` int(11) NOT NULL,
  `FirstName` varchar(255) NOT NULL,
  `LastName` varchar(255) NOT NULL,
  `Town` varchar(255) NOT NULL,
  `StreetName` varchar(255) NOT NULL,
  `Number` int(11) NOT NULL,
  `PostalCode` int(11) NOT NULL,
  `Age` int(11) NOT NULL,
  `DoctorId` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Triggers `patient`
--
DELIMITER $$
CREATE TRIGGER `doctors_patient_uniform_distribution_trigger` BEFORE INSERT ON `patient` FOR EACH ROW BEGIN
DECLARE max integer;
DECLARE min integer;
set @max := (SELECT COUNT(DoctorId) AS NumOfPatients FROM `patient` GROUP BY DoctorId ORDER BY COUNT(DoctorId) DESC LIMIT 1);
set @min := (SELECT COUNT(DoctorId) AS NumOfPatients FROM `patient` GROUP BY DoctorId ORDER BY COUNT(DoctorId) ASC LIMIT 1);
IF( (@max - @min >= 3) AND ((SELECT DoctorId AS NumOfPatients FROM `patient` GROUP BY DoctorId ORDER BY COUNT(DoctorId) DESC LIMIT 1) = NEW.DoctorId )) THEN
set NEW.DoctorId = (SELECT DoctorId AS NumOfPatients FROM `patient` GROUP BY DoctorId ORDER BY COUNT(DoctorId) ASC LIMIT 1);
END IF;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `pharmaceuticalcompany`
--

CREATE TABLE `pharmaceuticalcompany` (
  `PharmaceuticalCompanyId` int(11) NOT NULL,
  `Name` varchar(255) NOT NULL,
  `PhoneNumber` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `pharmacy`
--

CREATE TABLE `pharmacy` (
  `PharmacyId` int(11) NOT NULL,
  `Name` varchar(255) NOT NULL,
  `Town` varchar(255) NOT NULL,
  `StreetName` varchar(255) NOT NULL,
  `StreetNumber` int(11) NOT NULL,
  `PostalCode` int(11) NOT NULL,
  `PhoneNumber` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `prescription`
--

CREATE TABLE `prescription` (
  `PatientId` int(11) NOT NULL,
  `DoctorId` int(11) NOT NULL,
  `DrugId` int(11) NOT NULL,
  `Date` date NOT NULL,
  `Quantity` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Triggers `prescription`
--
DELIMITER $$
CREATE TRIGGER `current_prescription_valid_doctor_trigger` BEFORE INSERT ON `prescription` FOR EACH ROW BEGIN
IF ((SELECT DoctorId FROM `patient` WHERE PatientId=NEW.PatientId) <> NEW.DoctorId) THEN
SIGNAL SQLSTATE '45000' set message_text = 'Error: A patient can be perscripted only by his current doctor!';
END IF;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `sell`
--

CREATE TABLE `sell` (
  `PharmacyId` int(11) NOT NULL,
  `DrugId` int(11) NOT NULL,
  `Price` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Stand-in structure for view `updateable`
--
CREATE TABLE `updateable` (
`DoctorId` int(11)
,`FirstName` varchar(255)
,`LastName` varchar(255)
,`Speciality` varchar(255)
,`ExperienceYears` int(11)
);

-- --------------------------------------------------------

--
-- Structure for view `avgprice`
--
DROP TABLE IF EXISTS `avgprice`;

CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `avgprice`  AS  select `dr`.`DrugId` AS `DrugId`,`dr`.`Name` AS `DrugName`,round(avg(`s`.`Price`),2) AS `AvgPrice` from (`health_system`.`sell` `s` join `health_system`.`drug` `dr`) where (`s`.`DrugId` = `dr`.`DrugId`) group by `s`.`DrugId` ;

-- --------------------------------------------------------

--
-- Structure for view `updateable`
--
DROP TABLE IF EXISTS `updateable`;

CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `updateable`  AS  select `health_system`.`doctor`.`DoctorId` AS `DoctorId`,`health_system`.`doctor`.`FirstName` AS `FirstName`,`health_system`.`doctor`.`LastName` AS `LastName`,`health_system`.`doctor`.`Speciality` AS `Speciality`,`health_system`.`doctor`.`ExperienceYears` AS `ExperienceYears` from `health_system`.`doctor` where (`health_system`.`doctor`.`ExperienceYears` > 15) ;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `contract`
--
ALTER TABLE `contract`
  ADD PRIMARY KEY (`PharmacyId`,`PharmaceuticalCompanyId`),
  ADD KEY `PharmacyId` (`PharmacyId`),
  ADD KEY `PharmaceuticalCompanyId` (`PharmaceuticalCompanyId`);

--
-- Indexes for table `doctor`
--
ALTER TABLE `doctor`
  ADD PRIMARY KEY (`DoctorId`);

--
-- Indexes for table `drug`
--
ALTER TABLE `drug`
  ADD PRIMARY KEY (`DrugId`),
  ADD KEY `PharmaceuticalCompanyId` (`PharmaceuticalCompanyId`);

--
-- Indexes for table `patient`
--
ALTER TABLE `patient`
  ADD PRIMARY KEY (`PatientId`),
  ADD KEY `DoctorId` (`DoctorId`);

--
-- Indexes for table `pharmaceuticalcompany`
--
ALTER TABLE `pharmaceuticalcompany`
  ADD PRIMARY KEY (`PharmaceuticalCompanyId`);

--
-- Indexes for table `pharmacy`
--
ALTER TABLE `pharmacy`
  ADD PRIMARY KEY (`PharmacyId`);

--
-- Indexes for table `prescription`
--
ALTER TABLE `prescription`
  ADD PRIMARY KEY (`PatientId`,`DoctorId`,`DrugId`),
  ADD KEY `PatientId` (`PatientId`),
  ADD KEY `DoctorId` (`DoctorId`),
  ADD KEY `DrugId` (`DrugId`),
  ADD KEY `DoctorId_2` (`DoctorId`),
  ADD KEY `DoctorId_3` (`DoctorId`,`DrugId`);

--
-- Indexes for table `sell`
--
ALTER TABLE `sell`
  ADD PRIMARY KEY (`PharmacyId`,`DrugId`),
  ADD KEY `sell_restriction_drug` (`DrugId`),
  ADD KEY `PharmacyId` (`PharmacyId`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `doctor`
--
ALTER TABLE `doctor`
  MODIFY `DoctorId` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `drug`
--
ALTER TABLE `drug`
  MODIFY `DrugId` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `patient`
--
ALTER TABLE `patient`
  MODIFY `PatientId` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `pharmaceuticalcompany`
--
ALTER TABLE `pharmaceuticalcompany`
  MODIFY `PharmaceuticalCompanyId` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `pharmacy`
--
ALTER TABLE `pharmacy`
  MODIFY `PharmacyId` int(11) NOT NULL AUTO_INCREMENT;
--
-- Constraints for dumped tables
--

--
-- Constraints for table `contract`
--
ALTER TABLE `contract`
  ADD CONSTRAINT `fk_contract_pharmaceutical_comp_restr` FOREIGN KEY (`PharmaceuticalCompanyId`) REFERENCES `pharmaceuticalcompany` (`PharmaceuticalCompanyId`),
  ADD CONSTRAINT `fk_contract_pharmacy_restriction` FOREIGN KEY (`PharmacyId`) REFERENCES `pharmacy` (`PharmacyId`);

--
-- Constraints for table `drug`
--
ALTER TABLE `drug`
  ADD CONSTRAINT `fk_restriction_dpc` FOREIGN KEY (`PharmaceuticalCompanyId`) REFERENCES `pharmaceuticalcompany` (`PharmaceuticalCompanyId`);

--
-- Constraints for table `patient`
--
ALTER TABLE `patient`
  ADD CONSTRAINT `fk_restriction_dp` FOREIGN KEY (`DoctorId`) REFERENCES `doctor` (`DoctorId`);

--
-- Constraints for table `prescription`
--
ALTER TABLE `prescription`
  ADD CONSTRAINT `perscription_doct_restriction` FOREIGN KEY (`DoctorId`) REFERENCES `doctor` (`DoctorId`),
  ADD CONSTRAINT `perscription_drug_restriction` FOREIGN KEY (`DrugId`) REFERENCES `drug` (`DrugId`),
  ADD CONSTRAINT `perscription_patient_restriction` FOREIGN KEY (`PatientId`) REFERENCES `patient` (`PatientId`);

--
-- Constraints for table `sell`
--
ALTER TABLE `sell`
  ADD CONSTRAINT `sell_restriction_drug` FOREIGN KEY (`DrugId`) REFERENCES `drug` (`DrugId`),
  ADD CONSTRAINT `sell_restriction_pharmacy` FOREIGN KEY (`PharmacyId`) REFERENCES `pharmacy` (`PharmacyId`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
