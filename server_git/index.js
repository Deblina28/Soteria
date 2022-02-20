const express = require('express')
const app = express()
const port = 3001

//firebase test
var fb=require('firebase')
var appInit=fb.initializeApp({
apiKey: "#",
authDomain: "",
projectId: "",
storageBucket: "",
messagingSenderId: "",
appId: ""
})

var firebaseDB=fb.database()
var arr1 = new Array(5);  

var firebaseReader=fb.database().ref("test");
//firebase single data read 
firebaseReader.once("value", function(snapshot){
    var data= snapshot.val();
    for (let i in data){
        var item=JSON.stringify(data[i]);
        arr1[i]=item;
        
        console.log(arr1[i]);
    }
        

 
     app.get("/api", (req, res) => {
        res.json({ message: "Server is Live"});
        
      });

    
}
)

//firebase continuous data read 
/*
firebaseReader.on("value", function(snapshot){
    var data= snapshot.val();
    for(let i in data){
        snapshot.forEach(data[i]);
    }
})
*/


//firebase data write 
firebaseDB.ref("Test").set("test_string2")


app.get('/', (req, res) => {
  res.send('Hello World!')
})


  
app.listen(port, () => {
  console.log(`Example app listening on port ${port}`)
})

