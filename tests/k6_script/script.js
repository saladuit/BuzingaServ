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
  handleSummary(url, res);
}

export function handleSummary(url, response) {
  return {
        './log/HTML_body.log' : JSON.stringify(url),
        './log/HTML_header.log' : JSON.stringify(url),
  };
}
