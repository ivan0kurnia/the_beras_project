import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;

void main() => runApp(MyApp());

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
        home: Scaffold(
            appBar: AppBar(title: Text('Admin Registration Form')),
            body: Center(child: RegisterAdmin())));
  }
}

class RegisterAdmin extends StatefulWidget {
  RegisterAdminState createState() => RegisterAdminState();
}

class RegisterAdminState extends State {
  // Boolean variable for CircularProgressIndicator.
  bool visible = false;

  // Getting value from TextField widget.
  final admin_usernameController = TextEditingController();
  final admin_passwordController = TextEditingController();
  final admin_firstnameController = TextEditingController();
  final admin_lastnameController = TextEditingController();
  final admin_emailController = TextEditingController();
  final admin_phone_numberController = TextEditingController();

  Future AdminRegistration() async {
    // Showing CircularProgressIndicator.
    setState(() {
      visible = true;
    });

    // Getting value from Controller
    String admin_username = admin_usernameController.text;
    String admin_password = admin_passwordController.text;
    String admin_firstname = admin_firstnameController.text;
    String admin_lastname = admin_lastnameController.text;
    String admin_email = admin_emailController.text;
    String admin_phone_number = admin_phone_numberController.text;


    // Starting Web API Call.
    final response = await http.post("http://mejakalori.xyz/Beras/BerasApp/register_admin.php",
        body: {'admin_username': admin_username, 'admin_password': admin_password, 'admin_firstname': admin_firstname,'admin_lastname': admin_lastname, 'admin_email': admin_email, 'admin_phone_number': admin_phone_number});


    // If Web call Success than Hide the CircularProgressIndicator.
    if (response.statusCode == 200) {
      setState(() {
        visible = false;
      });
    }

    // Showing Alert Dialog with Response JSON Message.
    showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: new Text(response.body),
          actions: <Widget>[
            FlatButton(
              child: new Text("OK"),
              onPressed: () {
                Navigator.of(context).pop();
              },
            ),
          ],
        );
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        body: SingleChildScrollView(
            child: Center(
      child: Column(
        children: <Widget>[
          Padding(
              padding: const EdgeInsets.all(12.0),
              child: Text('Admin Registration Form',
                  style: TextStyle(fontSize: 21))),
          Divider(),
          Container(
              width: 280,
              padding: EdgeInsets.all(10.0),
              child: TextField(
                controller: admin_usernameController,
                autocorrect: true,
                decoration: InputDecoration(hintText: 'Enter Your username Here'),
              )),
          Container(
              width: 280,
              padding: EdgeInsets.all(10.0),
              child: TextField(
                controller: admin_passwordController,
                autocorrect: true,
                obscureText: true,
                decoration: InputDecoration(hintText: 'Enter Your password Here'),
              )),
          Container(
              width: 280,
              padding: EdgeInsets.all(10.0),
              child: TextField(
                controller: admin_firstnameController,
                autocorrect: true,
                decoration:
                    InputDecoration(hintText: 'Enter Your first name Here'),
              )),
          Container(
              width: 280,
              padding: EdgeInsets.all(10.0),
              child: TextField(
                controller: admin_lastnameController,
                autocorrect: true,
                decoration:
                InputDecoration(hintText: 'Enter Your last name Here'),
              )),
          Container(
              width: 280,
              padding: EdgeInsets.all(10.0),
              child: TextField(
                controller: admin_emailController,
                autocorrect: true,
                decoration:
                InputDecoration(hintText: 'Enter Your email Here'),
              )),
          Container(
              width: 280,
              padding: EdgeInsets.all(10.0),
              child: TextField(
                controller: admin_phone_numberController,
                autocorrect: true,
                decoration:
                InputDecoration(hintText: 'Enter Your phone number Here'),
              )),
          RaisedButton(
            onPressed: AdminRegistration,
            color: Colors.green,
            textColor: Colors.white,
            padding: EdgeInsets.fromLTRB(10, 10, 10, 10),
            child: Text('Click Here To Register Admin Online'),
          ),
          Visibility(
              visible: visible,
              child: Container(
                  margin: EdgeInsets.only(bottom: 30),
                  child: CircularProgressIndicator())),
        ],
      ),
    )));
  }
}
