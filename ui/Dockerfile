
FROM node:16-slim AS build-step
WORKDIR /app
COPY package.json package-lock.json ./
RUN npm install
COPY . .
RUN npm run build --prod

FROM nginx:alpine
COPY --from=build-step /app/dist/ui /usr/share/nginx/html