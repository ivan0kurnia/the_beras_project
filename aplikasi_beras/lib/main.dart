import 'dart:async';
import 'dart:convert';
import 'package:aplikasi_beras/AdminPage.dart';
import 'package:aplikasi_beras/AdminRegister.dart';
import 'package:aplikasi_beras/UserRegister.dart';
import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;

void main() => runApp(new MyApp());

String username = '';
String email = '';
String firstname = '';
String lastname = '';
String tinggiberas = '';

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return new MaterialApp(
        debugShowCheckedModeBanner: false,
        title: 'login',
        home: new MyHomePage(),
        routes: <String, WidgetBuilder>{
          '/AdminPage': (BuildContext context) => new AdminPage(
                username: username,
                firstname: firstname,
                lastname: lastname,
                email: email,
                tinggiberas: tinggiberas,
              ),
          '/AdminRegister': (BuildContext context) => new RegisterAdmin(),
          '/UserRegister': (BuildContext context) => new RegisterUser(),
        });
  }
}

class MyHomePage extends StatefulWidget {
  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  TextEditingController user = new TextEditingController();
  TextEditingController pass = new TextEditingController();

  String msg = '';

  Future<int> _login() async {
    final response = await http.post(
        "http://mejakalori.xyz/Beras/BerasApp/login.php",
        headers: {'Content-Type': 'application/x-www-form-urlencoded'},
        body: {'username': user.text.trim(), 'password': pass.text});

    if (response.body == 'NO_MATCHING_USER') {
      setState(() {
        msg = "Wrong username or password.";
      });
      return -1;
    }

    var userdata = json.decode(response.body);
    var insideUserdata = [
      'admin_username', //0F
      'admin_firstname', //1
      'admin_lastname', //2
      'admin_email', //3
      'admin_phone_number' //4
          'height_percentage' //5
    ];

    for (var datum = 0; datum < insideUserdata.length; ++datum) {
      print(userdata[insideUserdata[datum]]);
    }

    Navigator.pushReplacementNamed(context, '/AdminPage');

    setState(() {
      username = userdata['admin_username'];
      firstname = userdata['admin_firstname'];
      lastname = userdata['admin_lastname'];
      email = userdata['admin_email'];
      tinggiberas = userdata['height_percentage'];
    });
    return 0;
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Center(child: Text("BERAS")),
      ),
      body: Container(
        child: Center(
          child: Column(children: <Widget>[
            Text(
              "Login",
              style: TextStyle(fontSize: 18.0),
            ),
            TextField(
              controller: user,
              decoration: InputDecoration(hintText: 'Username'),
            ),
            TextField(
              controller: pass,
              obscureText: true,
              decoration: InputDecoration(hintText: 'Password'),
            ),
            RaisedButton(
              child: Text("Login"),
              onPressed: () {
                _login();
              },
            ),
            RaisedButton(
              child: Text("Admin Register"),
              onPressed: () {
                Navigator.pushReplacementNamed(context, '/AdminRegister');
              },
            ),
            RaisedButton(
              child: Text("User Register"),
              onPressed: () {
                Navigator.pushReplacementNamed(context, '/UserRegister');
              },
            ),
            Text(
              msg,
              style: TextStyle(fontSize: 20.0, color: Colors.red),
            ),
          ]),
        ),
      ),
    );
  }
}
