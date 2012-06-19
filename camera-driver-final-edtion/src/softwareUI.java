import java.awt.*;

import javax.imageio.ImageIO;
import javax.swing.*;

import java.awt.event.*;
import java.io.File;
import java.io.IOException;
import java.util.Timer;  
import java.util.TimerTask; 
import jxgrabkey.HotkeyConflictException;
import jxgrabkey.HotkeyListener;
import jxgrabkey.JXGrabKey;
import java.awt.event.KeyEvent;

public class softwareUI extends JFrame implements ItemListener {
	JFrame frame = new JFrame("摄像头光感控制");
	private TrayIcon trayIcon;
	private SystemTray systemTray;
	int width,height,screenHeight,screenWidth;
	JCheckBox checkbox1,checkbox2;
	JTextField tf;
	JLabel label;
	
	Timer t = new Timer();
	
	int key = KeyEvent.VK_PAGE_DOWN;
	int mask = KeyEvent.SHIFT_MASK;
	int  MY_HOTKEY_INDEX = 1;
	boolean hotkeyEventReceived = false;
	
    public softwareUI(){
    	Toolkit tk = Toolkit.getDefaultToolkit();
    	Dimension screenSize = tk.getScreenSize(); 
    	screenHeight = screenSize.height;
    	screenWidth = screenSize.width;
    	width = 400;
    	height = 250;
    	frame.setLocation((screenWidth-width)/2, (screenHeight-height)/2);
    	frame.setSize(width, height);
    	frame.setResizable(false);
    	frame.setVisible(true);
    	frame.setLayout(null);
    	frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    	systemTray = SystemTray.getSystemTray();
    	checkbox1 = new JCheckBox("是否使用热键 shift+PgDn 来调节亮度？");
    	checkbox2 = new JCheckBox("是否使用定时来调节亮度？");
    	tf = new JTextField();
    	label = new JLabel("分钟");
    	/*加入系统托盘图标并在系统托盘中加入程序*/
    	try {
			trayIcon = new TrayIcon(ImageIO.read(new File("trayicon.png")));			
			systemTray.add(trayIcon);			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}catch (AWTException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		/*组件加入面板*/
		frame.add(checkbox1);
		frame.add(checkbox2);
		frame.add(tf);
		frame.add(label);
		/*组件设置*/
		checkbox1.setFont(new Font("Serif",NORMAL,15));
		checkbox1.setBounds(20, 20, 350, 35);
		checkbox2.setFont(new Font("Serif",NORMAL,15));
		checkbox2.setBounds(20, 100, 350, 35);
		tf.setBounds(40, 145, 50, 30);
		tf.setVisible(false);
		label.setFont(new Font("Serif",NORMAL,15));
		label.setBounds(95, 145, 50, 30);
		label.setVisible(false);
		checkbox1.addItemListener(this);
		checkbox2.addItemListener(this);
		/*点击最小化程序最小化到托盘*/
		frame.addWindowListener(
			new WindowAdapter(){
			public void windowIconified(WindowEvent e){
				frame.dispose();
			}
			}
		);
        /*单击托盘图标还原窗口*/
		trayIcon.addMouseListener(
			new MouseAdapter(){
			public void mouseClicked(MouseEvent e){
				if(e.getClickCount() == 1){
				   frame.setExtendedState(JFrame.NORMAL);
				   frame.setVisible(true);
				}
			}
		    }
		);
		tf.addActionListener(new TFListener());
		System.loadLibrary("JXGrabKey");
		
    }

	@Override
	//复选框选择事件
	public void itemStateChanged(ItemEvent e) {		
		// TODO Auto-generated method stub
		if(e.getStateChange() == e.SELECTED){
			if(e.getSource() == checkbox1){
				//System.out.println("rrrrr");
				try {
					
					JXGrabKey.getInstance().registerAwtHotkey(MY_HOTKEY_INDEX,mask,key);
					
				} catch (HotkeyConflictException e1) {
					// TODO Auto-generated catch block
					JOptionPane.showMessageDialog(null, e1.getMessage(), e1.getClass().getName(), JOptionPane.ERROR_MESSAGE);
					JXGrabKey.getInstance().cleanUp();
					return;
				}
			    JXGrabKey.getInstance().addHotkeyListener(hotkeyListener);
			   /* while(!hotkeyEventReceived){
					try {
						Thread.sleep(1000);
					} catch (InterruptedException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
				}*/
			}
			else if(e.getSource() == checkbox2){			
			   tf.setVisible(true);
			   label.setVisible(true);
			}
			else{
				return;
			}
		}
		else{
			if(e.getSource() == checkbox1){
				JXGrabKey.getInstance().unregisterHotKey(MY_HOTKEY_INDEX);
				JXGrabKey.getInstance().removeHotkeyListener(hotkeyListener);
				JXGrabKey.getInstance().cleanUp();
			}
			else if(e.getSource() == checkbox2){		
				   tf.setVisible(false);
				   label.setVisible(false);
				   tf.setText(null);
				   t.cancel();
				   t = new Timer();
			}
			else{
				return;
			}
		}
	}
	
	HotkeyListener hotkeyListener = new jxgrabkey.HotkeyListener(){
		public void onHotkey(int hotkey_idx) {
			if (hotkey_idx != MY_HOTKEY_INDEX){
				return;
			}
			else{
				hotkeyEventReceived = true;
				//System.out.println("LLLLLL");
				try {
					Process proc = Runtime.getRuntime().exec("/home/xuwenqin/bbb.sh");			
				} catch (IOException e1) {
					e1.printStackTrace();
				} 
			}
		}
	};
	//文本框事件执行控制
	public class TFListener implements ActionListener{
		public void actionPerformed(ActionEvent e) {
		//	int time = Integer.parseInt(tf.getText());
			double time = Double.parseDouble(tf.getText());
			int time1;
			
			
			if(time <= 0||time > 60){
				JOptionPane.showConfirmDialog(null, "请输入0至60之间的数字", "警告", JOptionPane.DEFAULT_OPTION);
				tf.setText(null);
			}
			else if(tf.getText() == null){
				
			}
			else{
				
				JOptionPane.showConfirmDialog(null, "设置成功", "提示", JOptionPane.DEFAULT_OPTION);
				time = time*60*1000;
				time1 = (int)time;
				t.scheduleAtFixedRate(new TimerTask(){
					public void run(){
						try {
							Process proc = Runtime.getRuntime().exec("/home/xuwenqin/bbb.sh");			
						} catch (IOException e1) {
							e1.printStackTrace();
						} 
					}
				}, 0, time1);			
			}
		}
	} 	
}
