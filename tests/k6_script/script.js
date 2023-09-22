import http from 'k6/http';
import { sleep } from 'k6';




export default function () {

    const url = 'https://google.com';

    const res = http.get(url);

/*
    for (const p in res.headers) 
    {
        if (res.headers.hasOwnProperty(p))
        {
            console.log(p + ' : ' + res.headers[p]);
        }
  }
    */
  handleSummary(res);
}

export function handleSummary(response) {
  return {
        './log/HTML_body.log' : response.data.body(),
        './log/HTML_header.log' : response.data.allHeaders(),
  };
}
