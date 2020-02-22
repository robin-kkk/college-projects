package viewerPackage;

import java.awt.*;
import javax.swing.*;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.TableModel;
import javax.swing.table.TableColumn;

class GuiClassTable extends JPanel {

	//FIELDS
	private JTable classTable;
	private String[] columnNames = { "Name", "Type", "Access" };
	private DefaultTableCellRenderer celAlignCenter = new DefaultTableCellRenderer();
	private JScrollPane tableScrollPane;

	//CONSTRUCTOR
	GuiClassTable() {
		setBackground(Color.WHITE);
		celAlignCenter.setHorizontalAlignment(JLabel.CENTER);
	}

	//METHODS
	public void setClassTable(ClassInfo classInfo) {
		int numOfMethod = classInfo.getMethodNames().size();
		int numOfVariable = classInfo.getVariableNames().size();

		String[][] data = new String[numOfMethod + numOfVariable][3];

		for (int i = 0; i < numOfMethod; i++) {
			data[i][0] = classInfo.getMethodNames().get(i);
			data[i][1] = classInfo.getMethodTypes().get(i);
			data[i][2] = classInfo.getMethodAccess();
		}

		for (int i = numOfMethod, j = 0; i < (numOfMethod + numOfVariable); i++) {
			data[i][0] = classInfo.getVariableNames().get(j);
			data[i][1] = classInfo.getVariableTypes().get(j);
			data[i][2] = classInfo.getVariableAccess();
			j++;
		}

		classTable = new JTable(data, columnNames);

		classTable.getColumn("Name").setCellRenderer(celAlignCenter);
		classTable.getColumn("Type").setCellRenderer(celAlignCenter);
		classTable.getColumn("Access").setCellRenderer(celAlignCenter);
		classTable.setRowHeight(30);

		classTable.setFont(GuiMenu.FONT);
		classTable.setEnabled(false);
		classTable.setVisible(false);
		classTable.setAutoCreateRowSorter(true);
		classTable.setFillsViewportHeight(true);
		tableScrollPane = new JScrollPane(classTable);
		tableScrollPane.setPreferredSize(new Dimension((GuiMenu.WIDTH/3)*2 - 50, GuiMenu.HEIGHT - 60));
		add(tableScrollPane);
	}

	public JTable getClassTable() {
		return classTable;
	}
}
