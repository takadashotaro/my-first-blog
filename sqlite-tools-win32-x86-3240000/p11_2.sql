.mode column 
.headers on
SELECT bookId, name FROM bookCreator
INNER JOIN creator ON creator.id = creatorId
WHERE bookId=4;