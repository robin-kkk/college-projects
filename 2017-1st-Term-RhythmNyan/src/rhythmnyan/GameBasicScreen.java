package rhythmnyan;

import java.awt.Graphics;
import java.awt.Image;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.File;

import javax.sound.sampled.*;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JPanel;

public class GameBasicScreen extends JPanel implements MouseListener{

	//FIELDS
	private GameMain main;
	private JButton startBtn, exitBtn;
	
	private Image BasicScreenImg;
	
	private ImageIcon startBtnBasicImg;
	private ImageIcon startBtnPressedImg;
	private ImageIcon exitBtnBasicImg;
	private ImageIcon exitBtnPressedImg;
	
	private Clip bgClip;
	
	//CONSTRUCTOR
	public GameBasicScreen(GameMain main) {
		this.main = main;
		setBounds(0, 30, 1000, 620);
		setLayout(null);
		
		try {
			AudioInputStream ais = AudioSystem.getAudioInputStream(new File(main.path + "/Sound/backbgm.wav"));
			bgClip = AudioSystem.getClip();
			bgClip.open(ais);
			bgClip.start();
		} catch (Exception e) {
			e.printStackTrace();
		}

		String imagePath = main.path + "/Image/";
		BasicScreenImg = main.tk.getImage(imagePath + "GameBasicScreen/startScreenImg.png");
		startBtnBasicImg = new ImageIcon(imagePath + "GameBasicScreen/startBtnBasicImg.png");
		startBtnPressedImg = new ImageIcon(imagePath + "GameBasicScreen/startBtnPressedImg.png");
		exitBtnBasicImg = new ImageIcon(imagePath + "GameBasicScreen/endBtnBasicImg.png");
		exitBtnPressedImg = new ImageIcon(imagePath + "GameBasicScreen/endBtnPressedImg.png");
		
		startBtn = new JButton(startBtnBasicImg);
		startBtn.setBounds(600, 150, 100, 97);
		startBtn.setBorderPainted(false);
		startBtn.setContentAreaFilled(false);
		add(startBtn);
		startBtn.addMouseListener(this);
		
		exitBtn = new JButton(exitBtnBasicImg);
		exitBtn.setBounds(850, 300, 100, 97);
		exitBtn.setBorderPainted(false);
		exitBtn.setContentAreaFilled(false);
		add(exitBtn);
		exitBtn.addMouseListener(this);
		
	}
	
	//METHODS
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		g.drawImage(BasicScreenImg, 0, 0, this);
	}

	@Override
	public void mouseClicked(MouseEvent e) {
		try {
			AudioInputStream ais = AudioSystem.getAudioInputStream(main.clicksound);
			main.click = AudioSystem.getClip();
			main.click.open(ais);
			main.click.start();
		}catch(Exception e3) {
			e3.printStackTrace();
		}
		
		if(e.getSource().equals(startBtn)) {
			main.status = 1;
			bgClip.stop();
			bgClip.close();
			this.setFocusable(false);
			this.setVisible(false);
			main.update();
		}else if(e.getSource().equals(exitBtn)) {
			System.exit(0);
		}
	}

	@Override
	public void mousePressed(MouseEvent e) { }

	@Override
	public void mouseReleased(MouseEvent e) { }

	@Override
	public void mouseEntered(MouseEvent e) {
		try {
			AudioInputStream ais = AudioSystem.getAudioInputStream(main.catsound);
			main.enter = AudioSystem.getClip();
			main.enter.open(ais);
			main.enter.start();
		}catch(Exception e2) {
			e2.printStackTrace();
		}
		
		if(e.getSource().equals(startBtn)) {
			startBtn.setIcon(startBtnPressedImg);
		}else if(e.getSource().equals(exitBtn)) {
			exitBtn.setIcon(exitBtnPressedImg);
		}
	}

	@Override
	public void mouseExited(MouseEvent e) {
		if(main.enter.isRunning()) main.enter.stop();
		
		if(e.getSource().equals(startBtn)) {
			startBtn.setIcon(startBtnBasicImg);
			
		}else if(e.getSource().equals(exitBtn)) {
			exitBtn.setIcon(exitBtnBasicImg);
			
		}
	}
	
}
