SELECT DISTINCT name FROM people
RIGHT JOIN stars ON stars.person_id = people.id
WHERE NOT (name = 'Kevin Bacon' AND birth = 1958) AND stars.movie_id IN
(SELECT DISTINCT movie_id FROM stars WHERE person_id =
(SELECT DISTINCT id FROM stars INNER JOIN people ON stars.person_id = people.id WHERE name = 'Kevin Bacon' AND birth = 1958));
