import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'dart:async';
import 'dart:convert';
void main() {
  runApp(new MaterialApp(
    title: "User Tap History",
    home: new HistoryUser(),
  ));
}

class HistoryUser extends StatefulWidget {
  @override
  _AboutPageState createState() => new _AboutPageState();
}

class _AboutPageState extends State<HistoryUser> {
  List dataJSON;

  Future<String> ambildata() async {
    http.Response hasil = await http.get(
        Uri.encodeFull("http://mejakalori.xyz/Beras/BerasApp/user_tap_history.php"),
        headers: {"Accpet": "application/json"});

    this.setState(() {
      dataJSON = json.decode(hasil.body);
    });
  }

  @override
  void initState() {
    this.ambildata();
  }

  @override
  Widget build(BuildContext context) {
    return new Scaffold(
      appBar: new AppBar(
        title: new Text('User Tap History'),
      ),
      body: new ListView.builder(
        itemCount: dataJSON == null ? 0 : dataJSON.length,
        itemBuilder: (context, i) {
          return new Card(
              child:
              new Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: <Widget>[
                  Text('Nama :',
                      style: new TextStyle(fontSize: 20.0,color: Colors.blue)),
                  new Text(dataJSON[i]['name'],
                    style: new TextStyle(fontSize: 18.0),
                  ),
                  Text('Nomor UID :',
                      style: new TextStyle(fontSize: 20.0,color: Colors.blue)),
                  new Text(dataJSON[i]['card_uid'],
                    style: new TextStyle(fontSize: 18.0),
                  ),
                  Text('Tap date :',
                      style: new TextStyle(fontSize: 20.0,color: Colors.blue)),
                  new Text(dataJSON[i]['tap_datetime'],
                    style: new TextStyle(fontSize: 18.0),
                  ),
                ],
              )
          );
        },
      ),
    );
  }
}
