import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'dart:async';
import 'dart:convert';
void main() {
  runApp(new MaterialApp(
    title: "List View",
    home: new AdminList(),
  ));
}

class AdminList extends StatefulWidget {
  @override
  _AboutPageState createState() => new _AboutPageState();
}

class _AboutPageState extends State<AdminList> {
  List dataJSON;

  Future<String> ambildata() async {
    http.Response hasil = await http.get(
        Uri.encodeFull("http://mejakalori.xyz/Beras/BerasApp/get_admin_table.php"),
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
        title: new Text('List Admin'),
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
              new Text(dataJSON[i]['admin_name'],
              style: new TextStyle(fontSize: 18.0),
              ),
              Text('Email :',
                  style: new TextStyle(fontSize: 20.0,color: Colors.blue)),
              new Text(dataJSON[i]['admin_email'],
                style: new TextStyle(fontSize: 18.0),
              ),
              Text('Nomor Telpon :',
                  style: new TextStyle(fontSize: 20.0,color: Colors.blue)),
              new Text(dataJSON[i]['admin_phone_number'],
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
