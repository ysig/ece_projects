-- phpMyAdmin SQL Dump
-- version 4.5.1
-- http://www.phpmyadmin.net
--
-- Host: 127.0.0.1
-- Generation Time: Mar 08, 2017 at 01:32 AM
-- Server version: 10.1.19-MariaDB
-- PHP Version: 5.6.28

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `health_system`
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

--
-- Dumping data for table `contract`
--

INSERT INTO `contract` (`PharmacyId`, `PharmaceuticalCompanyId`, `StartDate`, `EndDate`, `Text`, `Supervisor`) VALUES
(2, 2, '2016-10-07', '2019-08-12', 'Lorem Ipsum', 'Olga Dymov'),
(2, 3, '2016-03-14', '2017-11-09', 'Lorem Ipsum', 'Konstantin Treplyov'),
(3, 1, '2016-09-10', '2019-06-13', 'Lorem Ipsum', 'Charlotta Ivanovna'),
(3, 2, '2016-12-11', '2020-09-30', 'Lorem Ipsum', 'Misail Poloznev'),
(3, 4, '2016-09-08', '2018-07-01', 'Lorem Ipsum', 'Semyon Yepikhodov'),
(4, 1, '2016-02-18', '2019-01-02', 'Lorem Ipsum', 'Andrei Kovrin'),
(4, 2, '2016-10-31', '2019-06-20', 'Lorem Ipsum', 'Yelena Andreyevna'),
(4, 3, '2016-12-06', '2018-12-21', 'Lorem Ipsum', 'Polina Andreyevna'),
(4, 4, '2016-11-28', '2018-11-02', 'Lorem Ipsum', 'Yermolai Lopakhin'),
(5, 2, '2016-05-09', '2020-05-04', 'Lorem Ipsum', 'Marya Yefimovna Grekova'),
(5, 3, '2016-08-25', '2020-02-26', 'Lorem Ipsum', 'Dmitri Gurov'),
(5, 4, '2016-01-27', '2018-11-25', 'Lorem Ipsum', 'Fyodor Kulygin'),
(6, 1, '2016-12-07', '2018-03-29', 'Lorem Ipsum', 'Ilya Ilyich Telegin'),
(6, 2, '2016-07-23', '2019-09-29', 'Lorem Ipsum', 'Olga Plemyannikov'),
(6, 4, '2016-06-11', '2018-10-10', 'Lorem Ipsum', 'Irina Sergeyevna');

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

--
-- Dumping data for table `doctor`
--

INSERT INTO `doctor` (`DoctorId`, `FirstName`, `LastName`, `Speciality`, `ExperienceYears`) VALUES
(1, 'Katerina', 'Verkhovtsev', 'Cardiologist', 15),
(2, 'Pavel', 'Smerdyakov', 'Endocrinologist', 7),
(3, 'Liza', 'Khokhlakov', 'Gastroenterologist', 24),
(4, 'Agrafena', 'Svetlov', 'Pediatrician', 15),
(5, 'Mikhail', 'Rakitin', 'Oncologist', 12),
(6, 'Pyotr', 'Miusov', 'Radiologist', 19),
(7, 'Kuzma', 'Samsonov', 'Pulmonologist', 26),
(8, 'Grifoty', 'Vasilievich', 'Surgeon', 9),
(10, 'Mario', 'Moretti', 'Agyrteia', 19);

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

--
-- Dumping data for table `drug`
--

INSERT INTO `drug` (`DrugId`, `Name`, `Formula`, `PharmaceuticalCompanyId`) VALUES
(1, 'SANDOSTATIN', 'Octreotide', 1),
(2, 'DUODART', 'Tamsulosin, Dutasteride', 3),
(3, 'HYCAMTIN', 'Topotecan', 3),
(4, 'LIPITOR', 'Atorvastatin', 2),
(5, 'AMOXIL', 'Amoxicillin', 3),
(6, 'YASMIN', 'Drospirenone, Ethinyl estradiol ', 4),
(7, 'GALVUS', 'Vildagliptin', 1),
(8, 'RELENZA', 'Zanamivir', 3),
(9, 'TASIGNA', 'Nilotinib', 1),
(10, 'LYRICA', 'Pregabalin', 2),
(11, 'DIOVAN', 'Valsartan', 1),
(12, 'FLIXOTIDE', 'Fluticasone', 3),
(13, 'XARELTO', 'Rivaroxaban', 4),
(14, 'EXJADE', 'Deferasirox', 1),
(15, 'CELEBREX', 'Celecoxib', 2),
(16, 'SUTENT', 'Sunitib', 2),
(17, 'CIPROXIN', 'Ciprofloxacin', 4),
(18, 'NEXAVAR', 'Sorafenib', 4),
(19, 'GiaTinPiesi', 'Piesiko', 2);

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
-- Dumping data for table `patient`
--

INSERT INTO `patient` (`PatientId`, `FirstName`, `LastName`, `Town`, `StreetName`, `Number`, `PostalCode`, `Age`, `DoctorId`) VALUES
(1, 'Stepan', 'Stepanovich', 'Athens', 'Pesmatzoglou', 4, 10559, 33, 3),
(2, 'Elisabeth', 'Alexeievna', 'Athens', 'Koumpari', 1, 10674, 59, 4),
(3, 'Yakov', 'Alpatych', 'Moschato', 'Konstantinoupoleos', 91, 18345, 42, 2),
(4, 'Maria', 'Dolokhova', 'Kalambaka', 'Xatzipetrou', 24, 42200, 34, 4),
(5, 'Tikhon', 'Shtcherbatov', 'Peristeri', 'Ethnikis Antistaseos', 24, 12134, 58, 7),
(6, 'Nikolai', 'Rostov', 'Ioannina', 'Kougkiou', 35, 45221, 41, 5),
(7, 'Maria', 'Hendrihovna', 'Agios Dimitrios', 'Dilou', 43, 17342, 29, 2),
(8, 'Mikhail', 'Ivanovich', 'Polichni', 'Rodou', 21, 56532, 37, 4),
(9, 'Anna', 'Malvintsev', 'Moschato ', 'Leoforos Poseidonos', 3, 18344, 31, 3),
(10, 'Pyotr', 'Shinshin', 'Agioi Anargyroi', 'Kefallinias', 56, 13561, 53, 5),
(11, 'Boris', 'Drubetskoy', 'Neapoli', 'Kallikratias', 32, 56728, 62, 1),
(12, 'Nikolay', 'Bolkonsky', 'Kilkis', 'Aristotelous', 24, 61100, 31, 6),
(13, 'Helene', 'Kuragina', 'Argiroupoli', 'Pelloponisou', 110, 16451, 44, 1),
(14, 'Makar', 'Bazdeyev', 'Larisa', 'Gr. Lambraki', 14, 41447, 18, 7),
(15, 'Nastasya', 'Ivanovna', 'Nafplio', 'Messologiou', 5, 21100, 67, 4);

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

--
-- Dumping data for table `pharmaceuticalcompany`
--

INSERT INTO `pharmaceuticalcompany` (`PharmaceuticalCompanyId`, `Name`, `PhoneNumber`) VALUES
(1, 'Novartis', 2103534436),
(2, 'Pfizer', 2107294025),
(3, 'GlaxoSmithKline', 2102839320),
(4, 'Bayer', 2105537290);

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

--
-- Dumping data for table `pharmacy`
--

INSERT INTO `pharmacy` (`PharmacyId`, `Name`, `Town`, `StreetName`, `StreetNumber`, `PostalCode`, `PhoneNumber`) VALUES
(1, 'Osip Vikhrov Apoteka', 'Larisa', 'Thoukididou', 5, 41222, 2147483647),
(2, 'Agrafya Yolkinova Apoteka', 'Ambelokipoi', 'Fillipoupoleos', 74, 56121, 2147483647),
(3, 'Anastasia Aslanova Apoteka', 'Athens', 'Xarilaou Trikoupi', 50, 10680, 2103665000),
(4, 'Petya Ismaylova Apoteka', 'Tripoli', 'Taxiarchon', 82, 22100, 2147483647),
(5, 'Rasputin Kuzubov Apoteka', 'Xanthi', 'Sarantaporou', 63, 67100, 2147483647),
(6, 'Rada Zykov Apoteka', 'Metsovo', 'Tositsa', 26, 44200, 2147483647);

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
-- Dumping data for table `prescription`
--

INSERT INTO `prescription` (`PatientId`, `DoctorId`, `DrugId`, `Date`, `Quantity`) VALUES
(1, 4, 8, '2016-01-27', 2),
(2, 6, 15, '2016-09-12', 4),
(4, 1, 4, '2016-06-22', 2),
(5, 4, 10, '2016-10-21', 4),
(6, 4, 10, '2016-09-02', 1),
(7, 5, 12, '2016-06-20', 3),
(10, 1, 8, '2016-05-18', 3),
(11, 5, 14, '2016-09-08', 2),
(12, 6, 4, '2016-12-27', 3),
(13, 4, 3, '2016-02-11', 3),
(13, 5, 1, '2016-10-14', 4),
(15, 6, 1, '2016-06-29', 4);

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

--
-- Dumping data for table `sell`
--

INSERT INTO `sell` (`PharmacyId`, `DrugId`, `Price`) VALUES
(1, 1, 50),
(1, 2, 34),
(1, 4, 20),
(2, 1, 44),
(2, 7, 7),
(2, 8, 24),
(2, 11, 1),
(3, 2, 13),
(3, 3, 33),
(3, 5, 1),
(3, 6, 25),
(3, 7, 38),
(3, 9, 5),
(3, 11, 13),
(4, 3, 29),
(4, 4, 16),
(4, 7, 19),
(4, 8, 10),
(4, 9, 42),
(4, 10, 19),
(4, 13, 45),
(5, 2, 49),
(5, 8, 34),
(5, 10, 47),
(5, 11, 1),
(6, 4, 5),
(6, 6, 7),
(6, 11, 33);

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

CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `avgprice`  AS  select `dr`.`DrugId` AS `DrugId`,`dr`.`Name` AS `DrugName`,round(avg(`s`.`Price`),2) AS `AvgPrice` from (`sell` `s` join `drug` `dr`) where (`s`.`DrugId` = `dr`.`DrugId`) group by `s`.`DrugId` ;

-- --------------------------------------------------------

--
-- Structure for view `updateable`
--
DROP TABLE IF EXISTS `updateable`;

CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `updateable`  AS  select `doctor`.`DoctorId` AS `DoctorId`,`doctor`.`FirstName` AS `FirstName`,`doctor`.`LastName` AS `LastName`,`doctor`.`Speciality` AS `Speciality`,`doctor`.`ExperienceYears` AS `ExperienceYears` from `doctor` where (`doctor`.`ExperienceYears` > 15) ;

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
  MODIFY `DoctorId` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=11;
--
-- AUTO_INCREMENT for table `drug`
--
ALTER TABLE `drug`
  MODIFY `DrugId` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=20;
--
-- AUTO_INCREMENT for table `patient`
--
ALTER TABLE `patient`
  MODIFY `PatientId` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=16;
--
-- AUTO_INCREMENT for table `pharmaceuticalcompany`
--
ALTER TABLE `pharmaceuticalcompany`
  MODIFY `PharmaceuticalCompanyId` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;
--
-- AUTO_INCREMENT for table `pharmacy`
--
ALTER TABLE `pharmacy`
  MODIFY `PharmacyId` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;
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
