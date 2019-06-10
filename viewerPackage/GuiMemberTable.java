package viewerPackage;

import java.awt.*;

import javax.swing.*;
import javax.swing.table.DefaultTableCellRenderer;

class GuiMemberTable extends JPanel {

	//FIELDS
	private JTable memberTable;
	private	String[] columnNames = {"Name", "Methods"};
	private String[][] data = new String[1][2];
	private StringBuilder methodNames = new StringBuilder();
	private DefaultTableCellRenderer celAlignCenter = new DefaultTableCellRenderer();
	private JScrollPane tableScrollPane;

	//CONSTRUCTOR
	GuiMemberTable() {
		setBackground(Color.WHITE);
		celAlignCenter.setHorizontalAlignment(JLabel.CENTER);
	}

	//METHODS
	public void setMemberTable(String valName, MethodInfo[] methodInfo) {
		
		for (int i = 0; i < methodInfo.length; i++){
			if(methodInfo[i].getBody().contains(valName)){
				methodNames.append(methodInfo[i].getName());
				if( i != methodInfo.length-1) methodNames.append(", ");
			}
		}
		
		data[0][0] = valName;
		data[0][1] = methodNames.toString();
		
		memberTable = new JTable(data,columnNames);
		
		memberTable.getColumn("Name").setCellRenderer(celAlignCenter);
		memberTable.getColumn("Name").setPreferredWidth(80);
		memberTable.getColumn("Methods").setCellRenderer(celAlignCenter);
		memberTable.getColumn("Methods").setPreferredWidth((GuiMenu.WIDTH/3) * 2 - 100);
		memberTable.setFont(GuiMenu.FONT);
		memberTable.setRowHeight(30);
		memberTable.setEnabled(false);
		memberTable.setVisible(false);
		memberTable.setFillsViewportHeight(true);
		memberTable.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);

		tableScrollPane = new JScrollPane(memberTable);
		tableScrollPane.setPreferredSize(new Dimension((GuiMenu.WIDTH/3)*2 - 50, GuiMenu.HEIGHT - 60));
		add(tableScrollPane);
	}
	
	public JTable getMemberTable() {
		return memberTable;
	}
}
	

