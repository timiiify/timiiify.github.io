echo -e "\033[32m 开始下载v2ray脚本\033[0m"
wget https://timiiify.github.io/v2.sh
echo -e "\033[32m 设置权限\033[0m"
chmod 777 v2.sh
echo -e "\033[32m 运行脚本\033[0m"
./v2.sh
echo -e "\033[32m 关闭v2ray服务\033[0m"
sudo systemctl disable v2ray --now
echo -e "\033[32m 添加公钥\033[0m"
wget -qO - https://apt.v2raya.org/key/public-key.asc | sudo tee /etc/apt/trusted.gpg.d/v2raya.asc
echo -e "\033[32m 添加 V2RayA 软件源\033[0m"
echo "deb https://apt.v2raya.org/ v2raya main" | sudo tee /etc/apt/sources.list.d/v2raya.list
sudo apt update
echo -e "\033[32m 安装v2raya\033[0m"
sudo apt install v2raya
echo -e "\033[32m 启动v2raya\033[0m"
sudo systemctl start v2raya.service
sudo systemctl enable v2raya.service
