SELECT DISTINCT title FROM movies
WHERE id IN
(SELECT DISTINCT movie_id FROM stars WHERE person_id =
(SELECT DISTINCT id FROM stars INNER JOIN people ON stars.person_id = people.id WHERE name = 'Bradley Cooper'))
AND id IN
(SELECT DISTINCT movie_id FROM stars WHERE person_id =
(SELECT DISTINCT id FROM stars INNER JOIN people ON stars.person_id = people.id WHERE name = 'Jennifer Lawrence'));
