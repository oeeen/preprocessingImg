package test;


import javax.swing.*;
import java.io.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.event.*;

import java.awt.Container;
import java.io.File;
import java.io.IOException;
import java.io.BufferedWriter;
import java.io.BufferedReader;
import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.util.ArrayList;
 
public class swing extends JFrame {
 
 private static final long serialVersionUID = -711163588504124217L;
 
 public swing() {
  super("swing");
  
  double totalCnt = 0.0;
  double corrCnt = 0.0;
  
  setBounds(100 , 100 , 300 , 200);
  setSize(300,410);
  setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
  
  Container contentPane = this.getContentPane();
  JPanel pane = new JPanel();
  JLabel text = new JLabel("Result");
  JTextArea tf = new JTextArea("");
  tf.setEditable(false);
  pane.add(text);
  pane.add(tf);
  
  String fileName = "/home/ilyoung/tt/preprocessingImg/output.txt";
  contentPane.add(pane);
  
  setVisible(true);
  
  try
  {
    FileReader fr = new FileReader(fileName);
    BufferedReader br = new BufferedReader(fr);
    String data = null;
    tf.setText("");
    while(true){
       data = br.readLine();
       if(data==null)
          break;
       tf.append(data+"\n");
    }
    br.close();
    fr.close();
  } 
  catch (IOException e)
  {
     
  }
 }
}

