(function () {

    // Your web app's Firebase configuration
    var firebaseConfig = {
        apiKey: "AIzaSyBwJtQU5gvW-q2g8NDC8ThDwaUysdNqC5s",
        authDomain: "firsttest-45a5a.firebaseapp.com",
        databaseURL: "https://firsttest-45a5a.firebaseio.com",
        projectId: "firsttest-45a5a",
        storageBucket: "firsttest-45a5a.appspot.com",
        messagingSenderId: "778897747305",
        appId: "1:778897747305:web:15ff7df248a8bb902e61f8",
        measurementId: "G-KGW3MZGLFC"
    };
    // Initialize Firebase
    firebase.initializeApp(firebaseConfig);
    firebase.analytics();

    // Get a reference to the database service
    //var database = firebase.database();
    firebase.database().ref('/object').on('value', function (snapshot) {
        document.getElementById('object').innerText = JSON.stringify(snapshot.val(), null, 3);
    });

    //firebase.database().ref().child('object').on('value', snap => console.log(snap.val()));


}());