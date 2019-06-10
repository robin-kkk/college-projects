package viewerPackage;

import java.util.ArrayList;
import java.util.StringTokenizer;

public class Parser {

	// FIELDS
	private ClassInfo classInfo = new ClassInfo();
	private ArrayList<MethodInfo> methodInfo = new ArrayList<>();
	private ArrayList<MemberData> memberData = new ArrayList<>();

	// CONSTRUCTOR
	Parser(StringBuffer buffer) {
		StringTokenizer st = new StringTokenizer(buffer.toString(), "\r\n", false);

		int methodOrder = 0;

		while (st.hasMoreTokens()) {
			String tmp = st.nextToken();

			if (tmp.contains(Keyword.CLASS)) {
				classInfo.setName(tmp.substring(tmp.indexOf(" ") + 1, tmp.length()));
				continue;
			}

			if (tmp.contains(");")) {
				methodInfo.add(new MethodInfo());
				memberData.add(new MemberData());
				continue;
			}

			if (buffer.indexOf(tmp) <= buffer.indexOf("};")) {
				if (tmp.lastIndexOf(':') == tmp.length() - 1) {
					for(String access : Keyword.ACCESS) {
						if (tmp.contains(access)) {
							if (buffer.indexOf(access) < buffer.indexOf("~")) {
								classInfo.setMethodAccess(access);
							} else {
								classInfo.setVariableAccess(access);
							}

							break;
						}
					}
				}

				if (buffer.indexOf(tmp) > buffer.indexOf("~")) {
					for(String type : Keyword.TYPE) {
						if (tmp.contains(type)) {
							if (tmp.indexOf("];") != -1) {
								classInfo.setVariableInfo(tmp.substring(tmp.lastIndexOf(" ") + 1, tmp.indexOf("[")),
										type + "[]");
							} else {
								classInfo.setVariableInfo(tmp.substring(tmp.lastIndexOf(" ") + 1, tmp.indexOf(";")),
										type);
							}

							break;
						}
					}
				}
			} else {
				if (tmp.contains("::") && !tmp.contains(Keyword.CONTROLSTATEMENT[0])) {
					StringBuffer methodName = new StringBuffer(tmp.substring(tmp.lastIndexOf(':') + 1, tmp.indexOf('(') + 1));
					String[] parameters;
					String paraType, paraName;

					for(String returnType : Keyword.TYPE) {
						if (tmp.contains(returnType + " ")) {
							if (!tmp.contains("()")) {
								if (tmp.contains(",")) {
									parameters = tmp.substring(tmp.indexOf('(') + 1, tmp.indexOf(')')).split(", ");

									for (int i = 0; i < parameters.length; i++) {
										paraType = parameters[i].split(" ")[0];
										paraName = parameters[i].split(" ")[1];
										methodName.append(paraType);

										if (i == parameters.length - 1) {
											methodName.append(')');
										} else {
											methodName.append(", ");
										}
										methodInfo.get(methodOrder).setParameter(paraName, paraType);
									}
								} else {
									methodName.append(tmp.substring(tmp.indexOf('(') + 1, tmp.lastIndexOf(' ')) + ')');
									paraType = tmp.substring(tmp.indexOf('(') + 1, tmp.lastIndexOf(' '));
									paraName = tmp.substring(tmp.lastIndexOf(' ') + 1, tmp.indexOf(')'));
									methodInfo.get(methodOrder).setParameter(paraName, paraType);
								}
							} else {
								methodName.append(")");
							}

							classInfo.setMethodInfo(methodName.toString(), returnType);
							methodInfo.get(methodOrder).setName(methodName.toString());
							methodOrder++;
							break;

						} else if (tmp.indexOf(classInfo.getName()) == 0) {
							if (!tmp.contains(Keyword.TYPE[0])) {
								if (tmp.contains(",")) {
									parameters = tmp.substring(tmp.indexOf('(') + 1, tmp.indexOf(')'))
											.split(", ");
									for (int i = 0; i < parameters.length; i++) {
										paraType = parameters[i].split(" ")[0];
										paraName = parameters[i].split(" ")[1];
										methodName.append(paraType);
										if (i == parameters.length - 1) {
											methodName.append(')');
										} else {
											methodName.append(", ");
										}
										methodInfo.get(methodOrder).setParameter(paraName, paraType);
									}
								} else {
									methodName.append(tmp.substring(tmp.indexOf('(') + 1, tmp.indexOf(' ')) + ')');
									paraType = tmp.substring(tmp.indexOf('(') + 1, tmp.lastIndexOf(' '));
									paraName = tmp.substring(tmp.lastIndexOf(' ') + 1, tmp.indexOf(')'));
									methodInfo.get(methodOrder).setParameter(paraName, paraType);
								}
							} else {
								methodName.append(")");
							}

							classInfo.setMethodInfo(methodName.toString(), Keyword.TYPE[0]);
							methodInfo.get(methodOrder).setName(methodName.toString());
							methodOrder++;
							break;
						}
					}
				} else {
					if (methodOrder > 0) {
						methodInfo.get(methodOrder - 1).setBody(tmp.substring(1));
						for (String val : classInfo.getVariableNames()) {
							if (tmp.contains(val)) {
								memberData.get(methodOrder - 1).addMemberData(val);
							}
						}
					}

				}
			}
		}
	}

	// METHODS
	public ClassInfo getClassInfo() {
		return classInfo;
	}
	public MethodInfo[] getMethodInfo() {
		return methodInfo.toArray(new MethodInfo[methodInfo.size()]);
	}
	public MemberData[] getMemberData() {
		return memberData.toArray(new MemberData[memberData.size()]);
	}
}
