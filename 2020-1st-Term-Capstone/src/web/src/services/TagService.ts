import axios from "axios";

class TagService {
  public async recommendByDefault() {
    const res = await axios.get(`${apiPath}tag/rmd_by_default/`);

    return res.data;
  }

  public async recommendByTags(tags: string[]) {
    const params = new URLSearchParams();
    params.append("tags", tags.join(" "));

    const res = await axios.post(`${apiPath}tag/rmd_by_tags/`, {
      tags: tags.join(" "),
    });

    return res.data;
  }
}

export default TagService;

const apiPath = "http://ec2-52-79-155-70.ap-northeast-2.compute.amazonaws.com/";
