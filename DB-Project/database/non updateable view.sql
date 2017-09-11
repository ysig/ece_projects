CREATE
 ALGORITHM = UNDEFINED
 VIEW `avgPrice`
 (DrugId, DrugName, AvgPrice)
 AS SELECT Dr.DrugId, Dr.Name, ROUND(AVG(S.Price), 2)
FROM sell S, drug Dr
WHERE S.DrugId=Dr.DrugId
GROUP BY S.DrugId