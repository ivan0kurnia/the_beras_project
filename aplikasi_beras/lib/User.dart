import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'dart:async';
import 'dart:convert';

void main() {
  runApp(new MaterialApp(
    title: "List View",
    home: new UserList(),
  ));
}

class UserList extends StatefulWidget {
  @override
  _AboutPageState createState() => new _AboutPageState();
}

class _AboutPageState extends State<UserList> {
  List dataJSON;

  Future<String> ambildata() async {
    http.Response hasil = await http.get(
        Uri.encodeFull(
            "http://mejakalori.xyz/Beras/BerasApp/get_user_table.php"),
        headers: {"Accpet": "application/json"});

    this.setState(() {
      dataJSON = json.decode(hasil.body);

      print(dataJSON[0]['phone_number']);
      print(dataJSON[0]['user_marital_status']);
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
        title: new Text('UserList'),
      ),
      body: new ListView.builder(
        itemCount: dataJSON == null ? 0 : dataJSON.length,
        itemBuilder: (context, i) {
          return new Card(
              child: new Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: <Widget>[
              Text('No.UID :',
                  style: new TextStyle(fontSize: 20.0, color: Colors.blue)),
              new Text(
                dataJSON[i]['card_uid'],
                style: new TextStyle(fontSize: 18.0),
              ),
              Text('Nama :',
                  style: new TextStyle(fontSize: 20.0, color: Colors.blue)),
              new Text(
                dataJSON[i]['user_name'],
                style: new TextStyle(fontSize: 18.0),
              ),
              Text('jenis kelamin :',
                  style: new TextStyle(fontSize: 20.0, color: Colors.blue)),
              new Text(
                dataJSON[i]['user_gender'],
                style: new TextStyle(fontSize: 18.0),
              ),
              Text('Nomor Telpon Rumah dan HP :',
                  style: new TextStyle(fontSize: 20.0, color: Colors.blue)),
              new Text(
              dataJSON[i]['phone_number'],
                style: new TextStyle(fontSize: 18.0),
              ),
              Text('alamat  yang ditinggal :',
                  style: new TextStyle(fontSize: 20.0, color: Colors.blue)),
              new Text(
                dataJSON[i]['user_address_current'],
                style: new TextStyle(fontSize: 18.0),
              ),

              Text('Tanggungan Keluarga :',
                  style: new TextStyle(fontSize: 20.0, color: Colors.blue)),
              new Text(
                dataJSON[i]['user_liability'],
                style: new TextStyle(fontSize: 18.0),
              ),
              Text('Beras yang Didapat  :',
                  style: new TextStyle(fontSize: 20.0, color: Colors.blue)),
              new Text(
                dataJSON[i]['user_staple_quota'],
                style: new TextStyle(fontSize: 18.0),
              ),
              Text('Tanggal Pendaftaran  :',
                  style: new TextStyle(fontSize: 20.0, color: Colors.blue)),
              new Text(
                dataJSON[i]['user_creation_date'],
                style: new TextStyle(fontSize: 18.0),
              ),
            ],
          ));
        },
      ),
    );
  }
}
