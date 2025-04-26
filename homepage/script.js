const ratingGroups = [1, 2, 3, 4];
let myVotes = [5, 4, 2, 5];
let userVotes = [];

ratingGroups.forEach(groupId => {
    const starRating = document.getElementById(`star-rating${groupId}`);
    const ratingValue = document.getElementById(`rating-value${groupId}`);
    console.log(starRating, ratingValue);

    starRating.addEventListener('change', function(e) {
        ratingValue.textContent = e.target.value;
        userVotes.push(e.target.value);
    });
});

document.getElementById('check').addEventListener('click', function(e) {
    let diff = 0;
    for (let i = 0; i < myVotes.length; i++) {
        console.log('partiamo')
        console.log(myVotes[i])
        console.log(userVotes[i])
        diff += Math.abs(myVotes[i] - userVotes[i]);
    }
    message='';
    if (diff < 5){
        message= 'Wow, we could totally start a book club together!';
    } else if (diff > 9){
        message= 'Our shelves would look different — and that’s totally fine.';
    } else {
        message= 'Not identical tastes, but we’d get along in a bookshop.';
    }
    document.querySelector('.p1').innerHTML = message;
    e.preventDefault();
});
