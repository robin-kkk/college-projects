package viewerPackage;

import javax.swing.*;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;
import java.util.ArrayList;

class GuiMenu extends JFrame implements ActionListener, TreeSelectionListener {

	// FIELDS
	public static final int WIDTH = 840;
	public static final int HEIGHT = 390;
	public static final Font FONT = new Font(Font.SANS_SERIF, Font.PLAIN, 18);

	private Parser parser;
	private ClassInfo classInfo;
	private MethodInfo[] methodInfo;
	private MemberData[] memberData;

	private String filePath=null;
	private StringBuffer buffer = new StringBuffer();

	private JFileChooser jFileChooser = new JFileChooser();
	private JMenuBar menuBar = new JMenuBar();
	private JMenu menu = new JMenu("File");
	private JMenuItem menuOpen, menuSave, menuSaveAs, menuExit;

	private JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT);
	private JSplitPane leftSplit = new JSplitPane(JSplitPane.VERTICAL_SPLIT);
	private JPanel treePanel = new JPanel();
	private JPanel leftPanel = new JPanel();

	private CardLayout card = new CardLayout();
	private CardLayout memberCard = new CardLayout();

	private JPanel[] panels;
	private JPanel tempPanel = new JPanel();
	private JPanel memberPanel = new JPanel();
	private JPanel rightPanel = new JPanel();

	private JScrollPane scrollPane;
	private JTree tree = new JTree();
	private DefaultTreeModel model;
	private DefaultMutableTreeNode root = new DefaultMutableTreeNode();

	private GuiClassTable classTablePanel = new GuiClassTable();
	private ArrayList<GuiTextArea> textAreaPanels = new ArrayList<>();
	private ArrayList<GuiMemberData> memberDataPanels = new ArrayList<>();
	private ArrayList<GuiMemberTable> memberTablePanels = new ArrayList<>();

	// CONSTRUCTOR
	GuiMenu() {
		setTitle("Class Viewer");
		setSize(WIDTH, HEIGHT);
		setResizable(false);
		setLocationRelativeTo(null);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		//Initialize JMenuBar
		menuBar.add(menu);

		menuOpen = new JMenuItem("Open");
		menuSave = new JMenuItem("Save");
		menuSaveAs = new JMenuItem("Save As");
		menuExit = new JMenuItem("Exit");
		menu.add(menuOpen);
		menu.addSeparator();
		menu.add(menuSave);
		menu.addSeparator();
		menu.add(menuSaveAs);
		menu.addSeparator();
		menu.add(menuExit);

		menuOpen.addActionListener(this);
		menuSave.addActionListener(this);
		menuSaveAs.addActionListener(this);
		menuExit.addActionListener(this);

		setJMenuBar(menuBar);

		//Initialize JTree
		model = new DefaultTreeModel(root);
		tree.setModel(model);
		scrollPane = new JScrollPane(tree);
		scrollPane.setPreferredSize(new Dimension(WIDTH/3, (HEIGHT/3)*2-55));
		
		tree.setFont(FONT);
		tree.setRootVisible(true);
		tree.setVisibleRowCount(10);
		tree.addTreeSelectionListener(this);

		//Initialize JPanel(rightPanel) including JPanels adding JTable(class), JTextAreas(method body), JTables(variable).
		rightPanel.setLayout(card);
		rightPanel.setBackground(Color.WHITE);

		//Initialize JPanel(leftPanel) including JPanels adding JTree, JTextAreas(memberData)
		treePanel.setBackground(Color.WHITE);
		treePanel.setMinimumSize(new Dimension(WIDTH/3, (HEIGHT/3)*2-50));
		treePanel.add(scrollPane);

		tempPanel.setBackground(Color.WHITE); // This show empty JPanel when not clicked method node
		memberPanel.setLayout(memberCard);
		memberPanel.setMinimumSize(new Dimension(WIDTH/3, HEIGHT/3));
		memberPanel.add(tempPanel);
		leftSplit.setTopComponent(treePanel);
		leftSplit.setBottomComponent(memberPanel);
		leftSplit.setDividerLocation((HEIGHT/3)*2-40);
		leftSplit.setDividerSize(5);
		leftPanel.setBackground(Color.WHITE);
		leftPanel.add(leftSplit);

		//Initialize JFrame's JSplitPane
		leftPanel.setMinimumSize(new Dimension(WIDTH/3, HEIGHT));
		rightPanel.setMinimumSize(new Dimension((WIDTH/3)*2-10, HEIGHT));

		splitPane.setLeftComponent(leftPanel);
		splitPane.setRightComponent(rightPanel);
		splitPane.setDividerLocation(WIDTH/3+10);
		splitPane.setDividerSize(10);
		splitPane.setOneTouchExpandable(true);
		
		setContentPane(splitPane);
		setVisible(true);

	}

	// METHODS
	public void setTree() {
		root.setUserObject(classInfo.getName());

		if (root.getChildCount() > 0) {
			root.removeAllChildren();
			model.reload();
		}

		for (int i = 0; i < classInfo.getMethodNames().size(); i++) {
			root.add(new DefaultMutableTreeNode(classInfo.getMethodNames().get(i)));
		}

		for (int i = 0; i < classInfo.getVariableNames().size(); i++) {
			root.add(new DefaultMutableTreeNode(
					classInfo.getVariableNames().get(i) + " : " + classInfo.getVariableTypes().get(i)));
		}

		tree.updateUI();
	}

	public void createPanel(int total) {
		panels = new JPanel[total];

		for (int i = 0; i < total; i++) {
			if (i == 0) {
				panels[0] = classTablePanel;
				rightPanel.add(panels[i], root.getUserObject().toString());
			} else {
				if (i < classInfo.getMethodNames().size() + 1) {
					panels[i] = textAreaPanels.get(i - 1);
					memberPanel.add(memberDataPanels.get(i - 1), model.getChild(root, i - 1).toString());
				} else {
					panels[i] = memberTablePanels.get(i - classInfo.getMethodNames().size() - 1);
				}
				rightPanel.add(panels[i], model.getChild(root, i - 1).toString());
			}
		}
	}

	@Override
	public void actionPerformed(ActionEvent arg0) {
		if (arg0.getSource() == menuOpen) {
			menuOpenMethod();
		} else if (arg0.getSource() == menuSave) {
			if(isModified()) menuSaveMethod();
		} else if (arg0.getSource() == menuSaveAs) {
			if(isModified()) menuSaveAsMethod();
		} else if (arg0.getSource() == menuExit) {
			System.exit(0);
		}
	}

	@Override
	public void valueChanged(TreeSelectionEvent e) {
		DefaultMutableTreeNode node = (DefaultMutableTreeNode) tree.getLastSelectedPathComponent();
		if(node == null) return;
		String nodeName = node.getUserObject().toString();
		int n = root.getIndex(node);

		memberCard.first(memberPanel);

		if (nodeName.equals(model.getRoot().toString())) {
			classTablePanel.getClassTable().setVisible(true);
		}else if (n >= 0 && n < parser.getMethodInfo().length) {
			memberCard.show(memberPanel, nodeName);
		} else if (n >= parser.getMethodInfo().length){
			memberTablePanels.get(n - parser.getMethodInfo().length).getMemberTable().setVisible(true);
		}

		card.show(rightPanel, nodeName);

	}

	public boolean isModified() { // This modify StringBuffer instance, "buffer" got from C++ file
		if(filePath==null){
			JOptionPane.showMessageDialog(this, "You should open your C++ file");
			return false;
		}

		StringBuffer pos = new StringBuffer();
		int start, end;
		for(int i = 0; i < methodInfo.length; i++) {
			pos.setLength(0);
			pos.append(classInfo.getName());
			pos.append("::");
			if( i < methodInfo.length -1) pos.append(methodInfo[i+1].getName().split("\\(")[0]);

			if(methodInfo[i].getBody().length() == 0) { // In case there's not the method body
				// Last number "+3" means length of String "\r\n}" of method body plus.
				start = (i == methodInfo.length - 1) ? buffer.lastIndexOf("{", buffer.length()-1) + 3
						: buffer.lastIndexOf("{", buffer.indexOf(pos.toString())) + 3;
				buffer.insert(start, textAreaPanels.get(i).getText());
			}else { // In case there's the method's body
				// Last number "+5" means replacing from String "\t" of method body's first line. p.s) "\r\n{\r\n\t"
				start = (i == methodInfo.length - 1) ? buffer.lastIndexOf("\r\n{", buffer.length()-1) + 5
						: buffer.lastIndexOf("\r\n{", buffer.indexOf(pos.toString())) + 5;
				end = (i == methodInfo.length - 1) ? buffer.length() - 1 : buffer.lastIndexOf("}", buffer.indexOf(pos.toString()));
				buffer.replace(start, end, textAreaPanels.get(i).getText());
			}
		}

		return true;
	}

	public void menuOpenMethod(){
		if (jFileChooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
			File f = jFileChooser.getSelectedFile();
			filePath = f.getAbsolutePath();
			FileInputStream file;

			if(buffer.length() > 0) {
				buffer.setLength(0);
				classTablePanel.removeAll();
				rightPanel.removeAll();
				textAreaPanels.clear();
				memberDataPanels.clear();
				memberTablePanels.clear();
			}

			try {
				file = new FileInputStream(filePath);
				int b = file.read();
				while (b != -1) {
					buffer.append((char) b);
					b = file.read();
				}
			} catch (FileNotFoundException e) {
				System.out.println("Oops : FileNotFoundException");
			} catch (IOException e) {
				System.out.println("Input error");
			}

			parser = new Parser(buffer);

			classInfo = parser.getClassInfo();
			methodInfo = parser.getMethodInfo();
			memberData = parser.getMemberData();

			setTree();
			classTablePanel.setClassTable(classInfo);

			for (int i = 0; i < methodInfo.length; i++) {
				GuiTextArea gta = new GuiTextArea();
				gta.setTextArea(methodInfo[i].getBody());
				textAreaPanels.add(gta);
				GuiMemberData gmd = new GuiMemberData();

				if (memberData[i].getMemberDatas() == null) {
					memberDataPanels.add(gmd);
					continue;
				}

				gmd.setMemberData(memberData[i]);
				memberDataPanels.add(gmd);
			}

			for (int i = 0; i < classInfo.getVariableNames().size(); i++) {
				GuiMemberTable memberTablePanel = new GuiMemberTable();
				memberTablePanel.setMemberTable(classInfo.getVariableNames().get(i), methodInfo);
				memberTablePanels.add(memberTablePanel);
			}

			createPanel(model.getChildCount(root) + 1);
		}
	}

	public void menuSaveMethod(){
		try {
			BufferedWriter bw = new BufferedWriter(new FileWriter(filePath));
			bw.write(buffer.toString());
			bw.close();
		}catch(IOException e) {
			e.printStackTrace();
		}
	}

	public void menuSaveAsMethod(){
		FileDialog dialog = new FileDialog(this, "SaveAs", FileDialog.SAVE);
		dialog.setDirectory(".");
		dialog.setVisible(true);

		if(dialog.getFile() == null) return;
		String dfName = dialog.getDirectory() + dialog.getFile();

		try {
			BufferedWriter bw = new BufferedWriter(new FileWriter(dfName));
			bw.write(buffer.toString());
			setTitle(dialog.getFile() + " -save " );
			bw.close();
		} catch (Exception e2) {
			JOptionPane.showMessageDialog(this, "Save Error");
		}
	}
}
