# WiFi Hacking <!-- omit in toc -->

- Course: Computer Security
- Language: None

This project was to have a presentation about security-related free topics, which includes whatever students research or develop or analyze etc.

In my case, the subject was how to hack a wireless network. PPT file is secret, so the content would be introduced here.

---

#### Table of contents <!-- omit in toc -->

- [Concept](#concept)
  - [Meanings of Wi-Fi](#meanings-of-wi-fi)
  - [IEEE 802.11 Standard](#ieee-80211-standard)
  - [Network Attack Techniques](#network-attack-techniques)
  - [Wireless Mode: Monitor Mode](#wireless-mode-monitor-mode)
- [Tools Required](#tools-required)
  - [USB Adapter: WNIC](#usb-adapter-wnic)
  - [Wireshark](#wireshark)
  - [Aircrack-ng](#aircrack-ng)
- [Application](#application)
  - [Basic: Open Netowrk](#basic-open-netowrk)
  - [Advanced: Encrypted Network](#advanced-encrypted-network)
  - [Others Scenario: Is it secure with SSL?](#others-scenario-is-it-secure-with-ssl)
- [Conclusion](#conclusion)

---

## Concept

### Meanings of Wi-Fi

Here is information defined by [Wikipedia](https://en.wikipedia.org/wiki/Wi-Fi).

> _Wi-Fi is a family of wireless networking technologies, based on the IEEE 802.11 family of standards, which are commonly used for local area networking of devices and Internet access._
> ...
> _Wi-Fi technology may be used to provide local network and Internet access to devices that are within Wi-Fi range of one or more routers that are connected to the Internet._
> ...
> _Wi-Fi uses multiple parts of the IEEE 802 protocol family, and is designed to interwork seamlessly with its wired sibling Ethernet. Compatible devices can network through a wireless access point to each other as well as to wired devices and the Internet. The different versions of Wi-Fi are specified by various IEEE 802.11 protocol standards, with the different radio technologies determining radio bands, and the maximum ranges, and speeds that may be achieved._

From this, we can know that

- Wi-Fi is a technology to let devices be connected in a wireless network, and this uses the IEEE 802.11 protocols.
- `Access Point` is a device to help some devices communicate with others in a wireless network, such as a router. Of course, this wired on Internet.
- `Station` is a device connected to Access Point, such as a smartphone, a laptop.

![1](./img/1.png?raw=true)

### IEEE 802.11 Standard

`Access Point` and `Station` communicate by the IEEE 802.11 protocols. So, they should keep the format of data packet according to it.

Before looking at the packet format, we need to understand the procedure to network between `Access Point` and `Station`.

**[Wi-Fi Protected Access (WPA/WPA2)](https://en.wikipedia.org/wiki/Wi-Fi_Protected_Access)**

WPA is alternatives of [WEP](https://en.wikipedia.org/wiki/Wired_Equivalent_Privacy), WPA2 replaced WPA. the [TKIP](https://en.wikipedia.org/wiki/Temporal_Key_Integrity_Protocol) is used for WPA, and the [AES](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard) is used for WPA2. Skipping the more details about encryption algorithm, WPA/WPA2 wireless protocols have a vulnerability in the `Authentication` and `Key Management` process.

| Data Encryption Protocol | Authentication | Encryption Algorithm |
| :----------------------: |:--------------:| :-------------------:|
| WPA      | PSK (individual), EAP (Enterprise) | TKIP |
| WPA2      | PSK (individual), EAP (Enterprise) | AES-CCMP |

- PSK is Pre-Shared Key between an Access point and a Staiton.

![2](./img/2.png?raw=true)

**Figure 1.** At first, each Access Point sends many beacon frames to the surrounding Stations. It's working to inform about oneself. So, each station sends a probe request packet to the desired Access Point. Then, the Access Point responses with a probe response packet. After that, some packet necessary for authentication is transferred with each other.

**Figure 2.** WPA/WPA2 need a key that only each other knows to encrypt and decrypt data. To generate the key, they perform the 4-way handshaking that packets are encrypted with a password of the connected wireless network. This is known as the [EAPOL-key protocol](http://www.hitchhikersguidetolearning.com/2017/09/17/eapol-4-way-handshake/).

**Figure 3.** In an encrypted network, authentication is worked with the EAPOL-key protocol. But in an open network, just exchange.

For now, we have figured out how devices are connected in a wireless network, and which packets are transferred with each other.

**[802.11 Frame Types](https://en.wikipedia.org/wiki/802.11_Frame_Types)**

Here is the packet format used in IEEE 802.11 standard protocols.

![3](./img/3.png?raw=true)

Packets are separated by the `Frame Control`.

![4](./img/4.png?raw=true)

- Type == 0: Mangament Frames
- Type == 1: Control Frames
- Type == 2: Data Frames
- SubType == 0: Association Request
- SubType == 1: Association Response
- SubType == 2: Reassociation Request
- SubType == 3: Reassociation Response
- SubType == 4: Probe Request
- SubType == 5: Probe Response
- SubType == 8: Beacon

More detail about format: [IEEE 802.11 Pocket Reference Guide](https://www.willhackforsushi.com/papers/80211_Pocket_Reference_Guide.pdf)

Anyway, other fields are explained as the followings by "Data and Computer Communications" of William Stallings.

> - **Frame Control.** Indicates the type of frame (control, management, or data) and provides control information. Control information includes whether the frame is to or from a DS, fragmentation information, and privacy information.
Duration/Connection ID: If used as a duration field, indicates the time (in microseconds) the channel will be allocated for successful transmission of a MAC frame. In some control frames, this field contains an association, or connection, identifier.
> - **Addresses.** The number and meaning of the 48-bit address fields depend on context. The transmitter address and receiver address are the MAC addresses of stations joined to the BSS that are transmitting and receiving frames over the wireless LAN. The service set ID (SSID) identifies the wireless LAN over which a frame is transmitted. For an IBSS, the SSID is a random number generated at the time the network is formed. For a wireless LAN that is part of a larger configuration the SSID identifies the BSS over which the frame is transmitted; specifically, the SSID is the MAC-level address of the AP for this BSS. Finally the source address and destination address are the MAC addresses of stations, wireless or otherwise, that are the ultimate source and 
destination of this frame. The source address may be identical to the transmitter address and the destination address may be identical to the receiver address.
> - **Sequence Control.** Contains a 4-bit fragment number subfield, used for frag- mentation and reassembly, and a 12-bit sequence number used to number frames sent between a given transmitter and receiver.
> - **Frame Body.** Contains an MSDU or a fragment of an MSDU. The MSDU is a LLC protocol data unit or MAC control information.
> - **Frame Check Sequence (FCS).** A 32-bit cyclic redundancy check.

In addition, an `RSN` field in this frame gives the type of network like WEP, WPA to us.

So, many tools related to network parse packets and handle the network connection between devices.

### Network Attack Techniques

There are some attack techniques with network.

- **Sniffing.** peeking out, bugging
- **Spoofing.** tricking, cheating, e.g. ARP Spoofing
- **Cracking + Brute Force Attack.** often used for digging into a password, a key
- **Man In The Middle Attack.** in the middle, intercepting and manipulating a packet from a sender and send to a responder.
- **Beacon Flooding Attack.** as one of denial-of-service attacks, creating a beacon frame corresponding to it of any access point and sending anywhere.

### Wireless Mode: Monitor Mode

There are 8 types of wireless mode.

- Master
- Ad-Hoc
- Infrastructure
- Repeater
- Mesh
- Wi-Fi direct
- TDLS
- Monitor

I would introduce only Monitor Mode in here. It's an excellent mode for Sniffing. Literally, that means to monitor all traffic going through a wireless channel between devices.

Monitor Mode is used for only a wireless network and can capture without connecting to an Access Point or Ad-Hoc network. Wireless packets move in the air, so it's able to monitor with a simple WNIC, too.

- WNIC stands for `Wireless Network Interface Card`.

---
## Tools Required

### USB Adapter: WNIC

We need to buy a USB adapter providing the Monitor Mode just like below.

![5](./img/5.png?raw=true)

- It's a product by the ipTime

The use is so simple. Just let's plug it into your computer and command `iwconfig` in Linux. And check the adapter name. Also, this adapter is so much cheap.

![6](./img/6.png?raw=true)

Next, change a mode of the adapter.

![7](./img/7.png?raw=true)

Good!

### Wireshark

This is a famous tool, which scans and sniffs in any network. If this tool is executed, you should select an adapter first, and then set up a few configurations for monitoring. A red box of the next image is to enter a keyword for filtering.

![8](./img/8.png?raw=true)

If you wanna more detail, click this [link](https://www.wireshark.org).

- [wireshark cheatsheet](https://www.comparitech.com/net-admin/wireshark-cheat-sheet/)
- [IEEE 802.11 Pocket Reference Guide](https://www.willhackforsushi.com/papers/80211_Pocket_Reference_Guide.pdf): This is of keywords information about the 802.11 packets.

### Aircrack-ng

This is an awesome open-source tool package for cracking or AP scanning or sniffing and so on in especially WLAN. Various useful tools are provided from it.

If you wanna more detail, click this [link](https://www.aircrack-ng.org/documentation.html).

---

## Application

- Notice the following handicaps.
  - This example doesn't apply an [SSL certificate](https://www.digicert.com/ssl/).
  - [HTTP Cookie](https://en.wikipedia.org/wiki/HTTP_cookie), which is meaningful data got from the monitored packets, is exploited for Account Hijacking.

### Basic: Open Netowrk

1. Let's adapt USB into your computer and change to Monitor Mode.
2. Scan using Airodump-ng and get `BSSID(=MAC address)` of the targeted Access Point.
![9](./img/9.png?raw=true)

3. Select a WNIC and enter the BSSID in Wireshark.
   - If you think that packets don't capture well, you can use Airmon-ng, which stop network-manager in Linux. If you can't access the Internet, it's a success.

      ``` text
      airmon-ng check kill
      ```

   - In addition, it needs to match the channel of the targeted Access Point. The channel is shown from Airodump-ng.

      ``` text
      airmon-ng start <interface> <channel>
      ```

4. Search packets to have HTTP Cookies of target.

![10](./img/10.png?raw=true)

5. Move the target site and set cookies with a browser extension `EditThisCookie`.
   - i.e. copy & paste from the found packet to the site.
![11](./img/11.png?raw=true)

6. What was going on is secret :p

### Advanced: Encrypted Network

Hello, welcome to the real world!

But, another handicap is added in this example. That's you should know the information about the network a victim belongs to, such as SSID and password.

Anyway, imagine that a victim keeps surfing on the Internet at a place like a cafe you belonged to.

As I said before, a wireless network has a somewhat complex authentication process. Do you remember that packets, which include things about the encryption key, is transferred between an Access Point and a Station. Yeah, they exchange the packets just before transferring data by the EAPOL-key protocol.

![12](./img/12.png?raw=true)

To sum up, you could see that the generated key finally needs to a `passphrase, SSID, Anonce, SNonce, AP MAC, Station MAC`.

If we observe EAPOL packets from first, we can find Nonce values. Then, what we don't know is the only `passphrase`, right? So, this is a single vulnerability that we can crack it using some tools.

In the EAPOL-key protocol, it's possible to find the key, which encrypt HTTP packets, by decrypting EAPOL packets with a password of the network a victim belongs to. That's why we have to know the information of the target WLAN, such as a password of a router.

After we get a key, we're able to observe HTTP packets in Wireshark, and then it's the same as behavior in an open network.

One more thing is that we should get the authentication procedure between an AP and a Station started again to get EAPOL packets. For this, we need to disconnect between them. This attack is known as the [Deauthentication Attack](https://en.wikipedia.org/wiki/Wi-Fi_deauthentication_attack). Also we can work this using `Aireplay-ng`.

```
aireplay-ng <interface> --deauth <packet count> -a <AP MAC>
```

![13](./img/13.png?raw=true)

After that, we can automatically perform decryption with [dot11decrypt](https://github.com/mfontanini/dot11decrypt), which takes all decrypted packets to a virtual adapter after catching EAPOL packets.

``` text
./dot11decrypt <interface> wpa:<ssid>:<password>
```

![14](./img/14.png?raw=true)

dot11decrypt prints like the following.

``` bash
Using device: tap0
Device is up
AP found: <SSID of target>: <MAC address of target>
```

Notice that `tap0` is the name of a virtual adapter, which dot11decrypt created. An attacker needs to just select `tap0` and watch in Wireshark.

In attacker's computer with a USB adapter and a virtual adapter (=`tap0`), one will monitor and another will wait decrypted packets from dot11decrypt after Deauthentication Attack.

**Summary**

1. Let's adapt USB into your computer and change to [`Monitor Mode`]().

2. Scan using Airodump-ng and get `BSSID(=MAC address)` of the targeted Access Point.

    ``` text
    airodump-ng <interface>
    ```

3. Select a WNIC and enter the BSSID in Wireshark.
   - If you think that packets don't capture well, you can use Airmon-ng, which stop network-manager in Linux. If you can't access the Internet, it's a success.

    ``` text
    airmon-ng check kill
    ```

   - In addition, it needs to match the channel of the targeted Access Point. The channel is shown from Airodump-ng.

    ``` text
    airmon-ng start <interface> <channel>
    ```

4. Work the Deauthentication Attack to the target.

    ``` text
    aireplay-ng <interface> --deauth <packet count> -a <AP MAC>
    ```

5. Execute dot11decrypt.

    ``` text
    ./dot11decrypt <interface> wpa:<ssid>:<password>
    ```

6. Enter the BSSID in Wireshark and search packets to have HTTP Cookies of target.
7. Move the target site and set cookies with a browser extension `EditThisCookie`.
   - Notice that the network-manager should be on.
   - i.e. copy & paste from the found packet to the site.
8. An attacker will get the account hijacking.
   - If you wonder about more detail, see the attached video.

### Others Scenario: Is it secure with SSL?

You'd remember what I referred to `Man-In-The-Middle Attack`.

In this case, an attacker intercepts https packets and sends a fake web page to a victim.

![15](./img/15.png?raw=true)

Then, a victim might give personal data to an attacker if he or she doesn't correctly check a URL or so.

The following is one case.

- ![16](./img/16.png?raw=true)

---
## Conclusion

We looked at some hacking examples in a wireless network so far. Many people might think that WPA/WPA2 is secure since it has a password or so. But, the important thing we have to remember is that we should protect our data oneself on the Internet. This attitude is so much troublesome to someone, whereas treated as meaningful trivial things too.

Lastly, please don't do that in any network.
