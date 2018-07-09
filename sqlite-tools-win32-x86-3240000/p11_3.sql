.mode column 
.headers on
SELECT book.title, creator.name FROM bookCreator
INNER JOIN book ON book.id = bookId
INNER JOIN creator ON creator.id = creatorId
WHERE bookId=4;

