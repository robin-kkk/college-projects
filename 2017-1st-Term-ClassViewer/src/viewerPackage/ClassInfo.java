package viewerPackage;

import java.util.ArrayList;

public class ClassInfo {

	//FIELDS
	private String name;
	private ArrayList<String> methodNames = new ArrayList<>();;
	private ArrayList<String> methodTypes = new ArrayList<>();;
	private String methodAccess;
	private ArrayList<String> variableNames = new ArrayList<>();;
	private ArrayList<String> variableTypes = new ArrayList<>();;
	private String variableAccess;

	//CONSTRUCTOR
	ClassInfo() { }

	//METHODS
	public void setName(String name) { this.name = name; }
	public void setMethodAccess(String access) {
		methodAccess = access;
	}

	public void setVariableAccess(String access) {
		variableAccess = access;
	}

	public void setMethodInfo(String name, String type) {
		methodNames.add(name);
		methodTypes.add(type);
	}

	public void setVariableInfo(String name, String type) {
		variableNames.add(name);
		variableTypes.add(type);
	}

	public String getName() { return name; }
	public String getMethodAccess() { return methodAccess; }
	public String getVariableAccess() { return variableAccess; }
	public ArrayList<String> getMethodNames() { return methodNames; }
	public ArrayList<String> getMethodTypes() { return methodTypes; }
	public ArrayList<String> getVariableNames() { return variableNames; }
	public ArrayList<String> getVariableTypes() { return variableTypes; }
	
}