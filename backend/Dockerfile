FROM node:16-slim

LABEL org.opencontainers.image.source https://github.com/berlingoqc/quintal

USER node

RUN mkdir -p /home/node/app

WORKDIR /home/node/app

COPY --chown=node package*.json ./

RUN npm ci

# Bundle app source code
COPY --chown=node . .

RUN npm run build

EXPOSE ${PORT}

CMD [ "node" ]

