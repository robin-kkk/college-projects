package viewerPackage;

import java.util.ArrayList;

public class MethodArgumentInfo {

    //FIELDS
    private ArrayList<String> paraNames = new ArrayList<>();
    private ArrayList<String> paraTypes = new ArrayList<>();
    private ArrayList<Object> arguments = new ArrayList<>();

    //CONSTRUCTOR
    MethodArgumentInfo() { }

    //METHODS
    public void setParameterInfo(String paraName, String paraType) {
        paraNames.add(paraName);
        paraTypes.add(paraType);
    }

    public void setArgument(Object argument) { arguments.add(argument); }

    public ArrayList<String> getParameterName() { return paraNames; }
    public ArrayList<String> getParameterType() { return paraTypes; }
    public ArrayList<Object> getArgument() { return arguments; }

}
