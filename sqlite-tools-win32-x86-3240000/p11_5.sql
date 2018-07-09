.mode column 
.headers on
SELECT publisher,AVG(price),SUM(price) FROM book
GROUP BY publisher;
