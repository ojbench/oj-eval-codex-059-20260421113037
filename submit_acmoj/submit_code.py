#!/usr/bin/env python3
import os, json, requests, sys

def main():
    token = os.environ.get('ACMOJ_TOKEN') or open(os.path.join(os.getcwd(), '.acmoj_token')).read().strip()
    if not token:
        print('Missing ACMOJ token', file=sys.stderr)
        sys.exit(1)
    problem_id = int(os.environ.get('ACMOJ_PROBLEM_ID', '1440'))
    path = sys.argv[1] if len(sys.argv) > 1 else 'src.hpp'
    with open(path, 'r', encoding='utf-8') as f:
        code = f.read()
    api = 'https://acm.sjtu.edu.cn/OnlineJudge/api/v1'
    headers = {
        'Authorization': f'Bearer {token}',
        'Content-Type': 'application/x-www-form-urlencoded',
        'User-Agent': 'ACMOJ-Python-Client/2.2'
    }
    data = {'language': 'cpp', 'code': code}
    r = requests.post(f'{api}/problem/{problem_id}/submit', headers=headers, data=data, timeout=15, proxies={'https': None, 'http': None})
    print(r.text)
    r.raise_for_status()
    j = r.json()
    sid = j.get('id')
    if sid:
        with open('/workspace/submission_ids.log', 'a') as lf:
            lf.write(json.dumps({'submission_id': sid})+'\n')
    print('submission_id:', sid)

if __name__ == '__main__':
    main()
