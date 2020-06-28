import axios from "axios";

class MainService {
  public async recommendByHottest() {
    const res = await axios.get(`${apiPath}main/rmd_by_hottest/`);

    return res.data;
  }

  public async recommendByRandom() {
    const res = await axios.get(`${apiPath}main/rmd_by_random/`);

    return res.data;
  }

  public async getHottest() {
    const res = await axios.get(`${apiPath}main/get_hottest/`);

    return res.data;
  }

  public async getGoldencross() {
    const res = await axios.get(`${apiPath}main/get_goldencross/`);

    return res.data;
  }

  public async recommendByItem(code: string) {
    const res = await axios.post(`${apiPath}main/rmd_by_item/`, {
      code,
    });

    return res.data;
  }
}

export default MainService;

const apiPath = "http://ec2-52-79-155-70.ap-northeast-2.compute.amazonaws.com/";
