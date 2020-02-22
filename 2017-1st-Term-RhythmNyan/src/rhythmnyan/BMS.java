package rhythmnyan;

import java.io.*;
import java.util.ArrayList;

import javax.sound.sampled.*;

public class BMS {

	//FIELDS
	private String bmspath;
	private File bms, music, avi, dat;
	private File[] bmsList, fileList;
	private FileReader fr;
	private FileWriter fw;
	private BufferedReader br;
	private BufferedWriter bw;
	private int highScore;
	private String splitStr1, splitStr2;
	private String temp;
	private AudioInputStream ais;
	private String loadingImgPath;
	
	//HEADER FIELD
	public String title;
	public String artist;
	public String genre;
	public int playlevel;
	public double bpm;
	public int LNtype;

	boolean hasKeySound;
	GameSound[]	WAV = new GameSound[256];
	GameSound gamebg, sound;

	
	//MAIN DATA FIELD
	private ArrayList<Integer>[] madi; 
	private int madiNum = 0;
	private int madiFirstNum = 0;
	private int madiLastNum = 0;
	private ArrayList<String>[] channel;
	private int channelNum = 0;
	private String text = "";
	private int wavnum = 0;
	
	//CONSTRUCTOR
	BMS(String filepath, String songname) {
		title = songname;
		bmspath = filepath + "/BMS/";
		bmsList = new File(bmspath).listFiles();
		
		for(int i=0; i < bmsList.length; i++) {
			
			if(bmsList[i].getName().contains(title)) {
				bmspath += bmsList[i].getName()+"/";
				fileList = new File(bmspath).listFiles();

				for(int j=0; j < fileList.length; j++) {
					System.out.println(fileList[j].getName());
					if(fileList[j].getName().contains(".avi")) {
						avi = new File(bmspath + title + ".avi");
					}
					//gameScoreSave
					if(fileList[j].getName().contains(".dat")) {
						dat = new File(bmspath + "savedata.dat");
						System.out.println("dat : " + dat);
						gameScoreLoad(dat);
					}
					
					if(fileList[j].getName().contains(".wav")) {
						music = new File(bmspath + title + ".wav");
						 System.out.println("music : " + music);
					}
					
					//loadingImg
					if(fileList[j].getName().contains(".png")) {
						loadingImgPath = bmspath + title + ".png";
					}
					//bms
					if(fileList[j].getName().contains(".bms")) {
						bms = new File(bmspath + title + ".bms");
						try {
							fr = new FileReader(bms);
							br = new BufferedReader(fr);
							while((temp = br.readLine()) != null) {
								if(temp.indexOf('#') != -1) {
									if(temp.indexOf(':') != -1) {
										if(temp.charAt(6) == ':') {
											madiLastNum = removeZero(temp.substring(1, 4));
										}
									}
								}
							}
							System.out.println("Madi Last Number: " + madiLastNum);
							
							madiLastNum += 1;
							madi = new ArrayList[madiLastNum];
							channel = new ArrayList[madiLastNum];
							
							for(int num = 0; num < madiLastNum; num++) {
								madi[num] = new ArrayList<>(40);
								channel[num] = new ArrayList(40);
							}
							
							fr.close();
							br.close();
							
							bmsParsing(bms);
							
						} catch (Exception e) {
							System.out.println("BMS.java - �떎�뙣");
							e.printStackTrace();
						}
					}
				}
			}
		}
	}
	
	//METHODS
	public ArrayList[] getMadi() { return madi; }
	public ArrayList[] getChannel() { return channel; }
	public int getMadiLastNum() { return madiLastNum; }
	public int getMadiFirstNum() { return madiFirstNum; }
	public int getDuration() { return (int)gamebg.clip.getMicrosecondLength() / 1000000; }
	public String getLoadingImg() { return loadingImgPath; }
	
	public void setSound(File filename) {
		sound = new GameSound(filename);
		sound.play();
	}

	public void musicplay() {
		try {
			gamebg = new GameSound(music);
			gamebg.play();
		} catch (NullPointerException e) {
			System.out.println(String.format("Error: %s", e));
			System.exit(-1);
		}
	}

	public void musicstop(boolean bgyes) {
		if(bgyes) gamebg.stop();
		else sound.stop();
	}
	
	public void bmsParsing(File bms) { //create Madi
		try {
			fr = new FileReader(bms);
			br = new BufferedReader(fr);
			
			while((temp = br.readLine()) != null) {
				if(temp.indexOf('#') != -1) {
					if(temp.charAt(0) == '#') {
						//header field parsing
						if(temp.contains("#GENRE")) {
							genre = temp.split(" ")[1];
						}else if(temp.contains("#ARTIST")) {
							artist = temp.split(" ")[1];
						}else if(temp.contains("#BPM")) {
							bpm = Double.parseDouble(temp.split(" ")[1]);
						}else if(temp.contains("#PLAYLEVEL")) {
							playlevel = Integer.parseInt(temp.split(" ")[1]);
						}else if(temp.contains("#LNTYPE")) {
							LNtype = Integer.parseInt(temp.split(" ")[1]);
						}else if(temp.contains("#VIDEOFILE")) {
							//vedio file
						}
					}
					
					if(temp.indexOf("#WAV") != -1) {
						if(temp.substring(1, 4).equalsIgnoreCase("WAV")) {
							String filename = temp.substring(7);
							if(filename.length() < 15) {
							wavnum = changehexint(temp.charAt(4)) * 16;
							wavnum += changehexint(temp.charAt(5));
							System.out.println("wavnum : " + wavnum);
							WAV[wavnum] = new GameSound(bmspath + "keysound/" + filename);
							System.out.println("wavnum : " + wavnum);
							if(hasKeySound == false) hasKeySound = true;
							}
						}
					}
					
					//main field parsing
					if(temp.indexOf(':') != -1) {
						if(temp.charAt(0) == '#') {
							if(temp.charAt(6) == ':') {

								text = temp.split(":")[1];
								
								splitStr1 = temp.substring(1, 4);
								madiNum = removeZero(splitStr1);
								
								splitStr2 = temp.substring(4, 6);
								channelNum = removeZero(splitStr2);

								madi[madiNum].add(channelNum);

								channel[madiNum].add(text);
							}
							//System.out.println("madi["+madiNum+"] : "+madi[madiNum]);
							//System.out.println("size : "+madi[madiNum].size());
						}
					}
				}	
			}
			//System.out.println("channel["+madiNum+"] : "+channel[madiNum]);
			//System.out.println("size : "+channel[madiNum].size());

			for(int i = 0; i<madiLastNum; i++) {
				if(madi[i].size() != 0) {
					madiFirstNum = i;
					System.out.println("madiFirstNum : " + madiFirstNum);
					break;
				}
			}
			
			fr.close();
			br.close();
			
		} catch (Exception e) {
			System.out.println("BMS.java - bmsParsing(method) error");
			e.printStackTrace();
		}
	}
	
	public void gameScoreLoad(File dat) {
		try {
			fr = new FileReader(dat);
			br = new BufferedReader(fr);
			while((temp = br.readLine()) != null) {
				if(temp.contains("#score")) {
					highScore = Integer.parseInt(temp.split(" ")[1]);
					break;
				}
			}
			
			fr.close();
			br.close();
		
		} catch (Exception e) {
			System.out.println("BMS.java - gameScoreLoad(method) error");
			e.printStackTrace();
		} 
	}
	
	public void gameScoreSave(int score, int maxCombo) {
		if(score > highScore) {
			try {
				fw = new FileWriter(dat);
				bw = new BufferedWriter(fw);
				bw.write("#score "+score);
				bw.newLine();
				bw.write("#maxcombo "+maxCombo);
				
				bw.close();
				fw.close();
			} catch (IOException e) {
				System.out.println("BMS.java - gameScoreSave(method) error");
				e.printStackTrace();
			}
		}
	}

	public int removeZero(String str) {
		while(str.charAt(0) == '0') {
			str = str.substring(1);
		}
		
		return Integer.parseInt(str);
	}

	public int changehexint(char temp){
		switch(temp){
			case '0':
				return 0;
			case '1':
				return 1;
			case '2':
				return 2;
			case '3':
				return 3;
			case '4':
				return 4;
			case '5':
				return 5;
			case '6':
				return 6;
			case '7':
				return 7;
			case '8':
				return 8;
			case '9':
				return 9;
			case 'a':case 'A':
				return 10;
			case 'b':case 'B':
				return 11;
			case 'c':case 'C':
				return 12;
			case 'd':case 'D':
				return 13;
			case 'e':case 'E':
				return 14;
			case 'f':case 'F':
				return 15;
			default:
				return 0;
		}
	}

	class GameSound {
		Clip clip;

		public  GameSound(String SoundFileName) {
			System.out.println("soundFileName : " + SoundFileName);
			try {
				File soundFile = new File(SoundFileName);
				AudioInputStream audioIn = AudioSystem.getAudioInputStream(soundFile);
				clip = AudioSystem.getClip();
				clip.open(audioIn);
			} catch (FileNotFoundException e) {
				return;
			} catch (UnsupportedAudioFileException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			} catch (LineUnavailableException e) {
				e.printStackTrace();
			}
		}
		public  GameSound(File SoundFileName) {
			try {
				AudioInputStream audioIn = AudioSystem
						.getAudioInputStream(SoundFileName);
				clip = AudioSystem.getClip();
				clip.open(audioIn);
			} catch (FileNotFoundException e) {
				return;
			}  catch (UnsupportedAudioFileException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			} catch (LineUnavailableException e) {
				e.printStackTrace();
			}
		}
		public void play() {
			if (clip.isRunning()){
				clip.stop();
			}
			clip.setFramePosition(0);
			clip.start();
		}
		public void stop() {
			clip.stop();
		}
	}
}
