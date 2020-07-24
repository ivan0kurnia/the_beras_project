import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;

void main() => runApp(MyApp());

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
        home: Scaffold(
            appBar: AppBar(title: Text('User Registration Form')),
            body: Center(child: RegisterUser())));
  }
}

class RegisterUser extends StatefulWidget {
  RegisterUserState createState() => RegisterUserState();
}

class RegisterUserState extends State {
  // Boolean variable for CircularProgressIndicator.
  bool visible = false;

  // Getting value from TextField widget.
  final card_uidController = TextEditingController();
  final user_ktpController = TextEditingController();
  final user_firstnameController = TextEditingController();
  final user_middlenameController = TextEditingController();
  final user_lastnameController = TextEditingController();
  final user_genderController = TextEditingController();
  final user_address_ktpController = TextEditingController();
  final user_address_currentController = TextEditingController();
  final user_landline_numberController = TextEditingController();
  final user_cellphone_numberController = TextEditingController();
  final user_religionController = TextEditingController();
  final user_marital_statusController = TextEditingController();
  final user_occupationController = TextEditingController();
  final user_birthplaceController = TextEditingController();
  final user_birthdateController = TextEditingController();
  final user_liabilityController = TextEditingController();
  final user_staple_quotaController = TextEditingController();

  Future userRegistration() async {
    // Showing CircularProgressIndicator.
    setState(() {
      visible = true;
    });

    // Getting value from Controller
    String card_uid = card_uidController.text;
    String user_ktp = user_ktpController.text;
    String user_firstname = user_firstnameController.text;
    String user_middlename = user_middlenameController.text;
    String user_lastname = user_lastnameController.text;
    String user_gender = user_genderController.text;
    String user_address_ktp = user_address_ktpController.text;
    String user_address_current = user_address_currentController.text;
    String user_landline_number = user_landline_numberController.text;
    String user_cellphone_number = user_cellphone_numberController.text;
    String user_religion = user_religionController.text;
    String user_marital_status = user_marital_statusController.text;
    String user_occupation = user_occupationController.text;
    String user_birthplace = user_birthplaceController.text;
    String user_birthdate = user_birthdateController.text;
    String user_liability = user_liabilityController.text;
    String user_staple_quota = user_staple_quotaController.text;


    // Starting Web API Call.
    final response = await http.post("http://mejakalori.xyz/Beras/BerasApp/register_user.php",
        body: {'card_uid': card_uid,
          'user_ktp': user_ktp,
          'user_firstname': user_firstname,
          'user_middlename': user_middlename,
          'user_lastname': user_lastname,
          'user_gender': user_gender,
          'user_address_ktp': user_address_ktp,
          'user_address_current': user_address_current,
          'user_landline_number': user_landline_number,
          'user_cellphone_number': user_cellphone_number,
          'user_religion': user_religion,
          'user_marital_status': user_marital_status,
          'user_occupation': user_occupation,
          'user_birthplace': user_birthplace,
          'user_birthdate': user_birthdate,
          'user_liability': user_liability,
          'user_staple_quota': user_staple_quota});


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
                      child: Text('User Registration Form',
                          style: TextStyle(fontSize: 21))),
                  Divider(),
                  Container(
                      width: 280,
                      padding: EdgeInsets.all(10.0),
                      child: TextField(
                        controller: card_uidController,
                        autocorrect: true,
                        decoration: InputDecoration(hintText: 'Enter Your card uid Here'),
                      )),
                  Container(
                      width: 280,
                      padding: EdgeInsets.all(10.0),
                      child: TextField(
                        controller: user_ktpController,
                        autocorrect: true,
                        decoration: InputDecoration(hintText: 'Enter ktp number Here'),
                      )),
                  Container(
                      width: 280,
                      padding: EdgeInsets.all(10.0),
                      child: TextField(
                        controller: user_firstnameController,
                        autocorrect: true,
                        decoration:
                        InputDecoration(hintText: 'Enter Your first name Here'),
                      )),
                  Container(
                      width: 280,
                      padding: EdgeInsets.all(10.0),
                      child: TextField(
                        controller: user_lastnameController,
                        autocorrect: true,
                        decoration: InputDecoration(hintText: 'Enter Your last name Here'),
                      )),
                  Container(
                      width: 280,
                      padding: EdgeInsets.all(10.0),
                      child: TextField(
                        controller: user_genderController,
                        autocorrect: true,
                        decoration:
                        InputDecoration(hintText: 'Enter Your gender (L/P) Here'),
                      )),
                  Container(
                      width: 280,
                      padding: EdgeInsets.all(10.0),
                      child: TextField(
                        controller: user_address_currentController,
                        autocorrect: true,
                        decoration: InputDecoration(hintText: 'Enter address current Here'),
                      )),
                  Container(
                      width: 280,
                      padding: EdgeInsets.all(10.0),
                      child: TextField(
                        controller: user_landline_numberController,
                        autocorrect: true,
                        decoration:
                        InputDecoration(hintText: 'Enter Your landline number Here'),
                      )),
                  Container(
                      width: 280,
                      padding: EdgeInsets.all(10.0),
                      child: TextField(
                        controller: user_cellphone_numberController,
                        autocorrect: true,
                        decoration:
                        InputDecoration(hintText: 'Enter Your cellphone number Here'),
                      )),
                  Container(
                      width: 280,
                      padding: EdgeInsets.all(10.0),
                      child: TextField(
                        controller: user_liabilityController,
                        autocorrect: true,
                        decoration:
                        InputDecoration(hintText: 'Enter Your liability Here'),
                      )),
                  RaisedButton(
                    onPressed: userRegistration,
                    color: Colors.green,
                    textColor: Colors.white,
                    padding: EdgeInsets.fromLTRB(10, 10, 10, 10),
                    child: Text('Click Here To Register User Online'),
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
