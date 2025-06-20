#!/usr/bin/bash
ntpdate ntp.aliyun.com
VIP="192.168.20.248"
while true; do
		if ip addr show | grep -q "$VIP";then
			echo "Current PC addr is VIP:$VIP"
			# 检测程序是否在运行
			if ! pgrep -x "coll" >/dev/null; then
					echo "程序已挂掉，重新启动..."
					# 启动程序的命令
					nohup /home/aiot/sgo/coll > /dev/null &  # 根据实际情况修改路径和启动命令

			else
					echo "coll already run"
			fi
		else
			sudo killall -9 coll
			echo "Current PC addr is not VIP:$VIP"
		fi



        sleep 5  # 休眠一段时间后再次检测
done
