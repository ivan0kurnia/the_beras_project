import 'package:flutter/material.dart';
import './AdminList.dart';
import './User.dart';
import './HistoryUser.dart';
import 'dart:io';

class AdminPage extends StatelessWidget {
  AdminPage(
      {this.username,
      this.firstname,
      this.lastname,
      this.email,
      this.phonenumber,
      this.tinggiberas});

  final String username;
  final String firstname;
  final String lastname;
  final String email;
  final String phonenumber;
  final String tinggiberas;

  @override
  Widget build(BuildContext context) {
    return new Scaffold(
      appBar: AppBar(title: Text("BERAS")),
      drawer: new Drawer(
        child: ListView(
          children: <Widget>[
            new UserAccountsDrawerHeader(
              accountName: new Text(
                '$firstname $lastname',
                style: TextStyle(fontSize: 20.0),
              ),
              accountEmail: new Text(email),
              currentAccountPicture: new CircleAvatar(
                backgroundImage: new NetworkImage('http://i.pravatar.cc/300'),
              ),
            ),
            new ListTile(
              title: new Text('Admin List'),
              onTap: () {
                Navigator.of(context).pop();
                Navigator.push(
                    context,
                    new MaterialPageRoute(
                        builder: (BuildContext context) => new AdminList()));
              },
            ),
            new Divider(
              color: Colors.black,
              height: 5.0,
            ),
            new ListTile(
              title: new Text('User List'),
              onTap: () {
                Navigator.of(context).pop();
                Navigator.push(
                    context,
                    new MaterialPageRoute(
                        builder: (BuildContext context) => new UserList()));
              },
            ),
            new Divider(
              color: Colors.black,
              height: 5.0,
            ),
            new ListTile(
              title: new Text('User Tap History'),
              onTap: () {
                Navigator.of(context).pop();
                Navigator.push(
                    context,
                    new MaterialPageRoute(
                        builder: (BuildContext context) => new HistoryUser()));
              },
            ),
            new Divider(
              color: Colors.black,
              height: 5.0,
            ),
            new ListTile(
              title: new Text('Exit Program'),
              onTap: () => exit(0),
            ),
          ],
        ),
      ),
      body: Column(
        children: <Widget>[
          Text(
            'Hello Admin\n$firstname $lastname,',
            style: TextStyle(fontSize: 20.0),
          ),
          Text('Persentase isi beras: $tinggiberas %',
              style: new TextStyle(fontSize: 20.0, color: Colors.blue)),
        ],
      ),
    );
  }
}
