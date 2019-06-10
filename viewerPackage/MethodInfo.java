package viewerPackage;

import java.util.ArrayList;

public class MethodInfo {

    //FIELDS
    private String name;
    private StringBuffer body = new StringBuffer();
    private MethodArgumentInfo mai = new MethodArgumentInfo();

    //CONSTRUCTOR
    MethodInfo() { }

    //METHODS
    public void setName(String name) { this.name = name; }

    public void setParameter(String paraName, String paraType) {
        mai.setParameterInfo(paraName, paraType);
    }

    public void setBody(String tmp) {
        body.append(tmp);
        body.append("\r\n");
    }

    public String getName() { return name; }

    public ArrayList<String> getParameterName() { return mai.getParameterName().isEmpty() ? null : mai.getParameterName(); }
    public ArrayList<String> getParameterType() { return mai.getParameterType().isEmpty() ? null : mai.getParameterType(); }
    public ArrayList<Object> getArgument() { return mai.getArgument().isEmpty() ? null : mai.getArgument(); }
    public String getBody() { return (body.length() == 0) ? "" : body.substring(0, body.length()-1); }

}
